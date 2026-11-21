-- MAME -autoboot_script; keep this callback alive for the full session.
local screen = manager.machine.screens[":video:screen"]


local frame = 0
local stop = tonumber(os.getenv("GAMATE_FRAMES")) or 1200
local psg = assert(io.open(os.getenv("GAMATE_PSG"), "w"))
local program = manager.machine.devices[":maincpu"].spaces["program"]
-- Drive the active-low bus on the same frame boundary as retro_input_poll.
-- UI input fields latch at the next frontend update, which can add a frame.
_G.gamate_input = program:install_read_tap(0x4400,0x47ff,"input",function()
    if frame>=780 and frame<786 then return 0xbf end
    if frame>=810 and frame<816 then return 0xef end
    return 0xff
end)
_G.gamate_psg = program:install_write_tap(0x4000,0x43ff,"ay",function(address,data)
    if frame >= stop then return end
    psg:write(manager.machine.time:as_ticks(2216500), " ", address & 15, " ", data, "\n")
end)
local output = assert(io.open(os.getenv("GAMATE_CAPTURE"), "wb"))
local checkpoints = {[60]=true,[180]=true,[300]=true,[360]=true,[600]=true,[900]=true,[1200]=true,[stop]=true}
_G.gamate_reference = emu.add_machine_frame_notifier(function()
    frame = frame + 1
    -- The frame notifier follows the texture swap. pixels() sees the back buffer.
    if checkpoints[frame-1] then
        output:write((screen:pixels()))
        output:flush()
    end
    if frame == stop+1 then
        output:close()
        psg:close()
        manager.machine:exit()
    end
end)
