"""Run an installed desktop RetroArch with isolated configuration and user ROMs."""
import argparse
from pathlib import Path
import subprocess

def main():
    p=argparse.ArgumentParser()
    for name in ("retroarch","core","system","game","output"):
        p.add_argument(name,type=Path)
    args=p.parse_args()
    root=args.output.resolve()
    root.mkdir(parents=True,exist_ok=True)
    opts=root/"Gamate"/"Gamate.opt"
    opts.parent.mkdir(exist_ok=True)
    fw=args.system.resolve()
    game=args.game.resolve()
    config=root/"retroarch.cfg"
    directories=["savefile_directory","savestate_directory","screenshot_directory",
                 "cache_directory","core_assets_directory","log_dir","runtime_log_directory",
                 "playlist_directory","core_info_path","rgui_config_directory"]
    config.write_text("\n".join(f'{k} = "{root.as_posix()}"' for k in directories)+
        f'\nsystem_directory = "{fw.as_posix()}"\ncore_options_path = "{opts.as_posix()}"\n'+
        'config_save_on_exit = "false"\nvideo_vsync = "false"\naudio_driver = "null"\n'
        'video_driver = "gdi"\nvideo_fullscreen = "false"\nvideo_windowed_fullscreen = "false"\n'
        'video_scale = "1"\nrewind_enable = "true"\nrewind_buffer_size = "16"\n'
        'savestate_auto_save = "true"\nsavestate_auto_load = "true"\n'
        'auto_overrides_enable = "false"\nhistory_list_enable = "false"\nglobal_core_options = "true"\n'
        'menu_show_start_screen = "false"\n',encoding="utf8")
    for name,bios,palette,ghost in [("cold","bit","game","medium"),("resume","bit","gray","strong"),
                                    ("umc","umc","mame","off")]:
        opts.write_text(f'gamate_bios = "{bios}"\ngamate_palette = "{palette}"\ngamate_ghost = "{ghost}"\n')
        shot=root/(name+".png")
        flags=["--max-frames=910" if name=="cold" else "--max-frames=80","--max-frames-ss",
               "--max-frames-ss-path="+str(shot)]
        cmd=[args.retroarch.resolve(),"--verbose","--log-file",root/(name+".log"),
             "-c",config,"-L",args.core.resolve(),*flags,game]
        subprocess.run(cmd,cwd=root,check=True,timeout=90)
        log=(root/(name+".log")).read_text(encoding="utf8")
        if ("Loaded "+bios.upper()+" BIOS") not in log: raise RuntimeError("Wrong BIOS: "+name)
        if not shot.exists() or not shot.stat().st_size: raise RuntimeError("No screenshot: "+name)
    print("Captured",root)
if __name__=="__main__":
    main()
