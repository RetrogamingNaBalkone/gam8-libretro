"""Run desktop RetroArch with isolated configuration and user-supplied content."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile


def core_state(path):
    """Extract the libretro core payload from an uncompressed RetroArch state."""
    data = path.read_bytes()
    if data.startswith(b"RASTATE\x01"):
        pos = 8
        while pos + 8 <= len(data):
            tag = data[pos:pos + 4]
            size = int.from_bytes(data[pos + 4:pos + 8], "little")
            pos += 8
            if pos + size > len(data):
                break
            if tag == b"MEM " and size:
                return data[pos:pos + size]
            pos += (size + 7) & ~7
    raise RuntimeError(f"Missing or truncated core state: {path}")


def main():
    parser = argparse.ArgumentParser()
    for name in ("retroarch", "core", "system", "game", "output"):
        parser.add_argument(name, type=Path)
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="smoke-", dir=output) as scratch:
        root = Path(scratch)
        try:
            check_runs(args, root)
        except Exception:
            for log in root.glob("*/retroarch.log"):
                print(log.parent.name, log.read_text(encoding="utf8", errors="replace")[-2500:])
            raise


def check_runs(args, root):

    def run(name, bios, palette, ghost, frames, previous_state=None):
        directory = root / name
        directory.mkdir()
        opts = directory / "Gam8" / "Gam8.opt"
        opts.parent.mkdir()
        opts.write_text(
            f'gamate_bios = "{bios}"\ngamate_palette = "{palette}"\ngamate_ghost = "{ghost}"\n',
            encoding="utf8",
        )
        if previous_state is not None:
            shutil.copyfile(previous_state, opts.parent / previous_state.name)
        config = directory / "retroarch.cfg"
        directories = ("savefile_directory", "savestate_directory", "screenshot_directory",
                       "cache_directory", "core_assets_directory", "log_dir", "runtime_log_directory",
                       "playlist_directory", "core_info_path", "rgui_config_directory")
        config.write_text(
            "".join(f'{key} = "{directory.as_posix()}"\n' for key in directories)
            + f'system_directory = "{args.system.resolve().as_posix()}"\n'
            + f'core_options_path = "{opts.as_posix()}"\n'
            + 'config_save_on_exit = "false"\nvideo_vsync = "false"\naudio_driver = "null"\n'
              'video_driver = "gdi"\nvideo_fullscreen = "false"\nvideo_windowed_fullscreen = "false"\n'
              'video_scale = "1"\ninput_driver = "null"\ninput_joypad_driver = "null"\n'
              'rewind_enable = "true"\nrewind_buffer_size = "16"\n'
              'savestate_auto_save = "true"\nsavestate_auto_load = "true"\n'
              'sort_savestates_enable = "true"\nsavestate_file_compression = "false"\nuser_language = "0"\n'
              'auto_overrides_enable = "false"\nhistory_list_enable = "false"\nglobal_core_options = "true"\n'
              'menu_show_start_screen = "false"\n',
            encoding="utf8",
        )
        shot = directory / "frame.png"
        log_path = directory / "retroarch.log"
        cmd = [args.retroarch.resolve(), "--verbose", "--log-file", log_path,
               "-c", config, "-L", args.core.resolve(), f"--max-frames={frames}",
               "--max-frames-ss", f"--max-frames-ss-path={shot}", args.game.resolve()]
        subprocess.run(cmd, cwd=directory, check=True, timeout=90)
        log = log_path.read_text(encoding="utf8")
        if f"Loaded {bios.upper()} BIOS" not in log or "[ERROR] [State]" in log:
            raise RuntimeError(f"BIOS or save-state failure: {log_path}")
        if previous_state is not None and not any(
            "Auto-loading" in line and previous_state.name in line and "succeeded" in line
            for line in log.splitlines()
        ):
            raise RuntimeError(f"RetroArch did not auto-load the cold save: {log_path}")
        if not shot.is_file() or not shot.read_bytes().startswith(b"\x89PNG\r\n\x1a\n"):
            raise RuntimeError(f"Missing PNG screenshot: {shot}")
        states = list(directory.rglob("*.state.auto"))
        if len(states) != 1 or not states[0].stat().st_size:
            raise RuntimeError(f"Expected one nonempty auto-save in {directory}")
        return states[0]

    cold_frames, resume_frames = 910, 80
    cold = run("cold", "bit", "game", "medium", cold_frames)
    resume = run("resume", "bit", "gray", "strong", resume_frames, cold)
    # The asynchronous auto-load can complete after the first frontend frame.
    # Compare with a cold boot of exactly the same duration and options.
    control = run("cold-control", "bit", "gray", "strong", resume_frames)
    resumed = core_state(resume)
    if resumed in (core_state(cold), core_state(control)):
        raise RuntimeError(f"Auto-load did not advance the saved game: {resume}")
    core_state(run("umc", "umc", "mame", "off", resume_frames))
    print("Verified both BIOS revisions and save-state continuation.")


if __name__ == "__main__":
    main()
