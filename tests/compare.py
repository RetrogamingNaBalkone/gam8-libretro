"""Compare LCD pixels, timestamped AY writes and signed 16-bit audio against MAME."""
import argparse
from array import array
import sys
import wave
import math
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path
import subprocess

def compare_audio(target):
    with wave.open(str(target/"mame.wav"), "rb") as w:
        assert (w.getnchannels(),w.getsampwidth(),w.getframerate())==(2,2,48000)
        reference=array("h",w.readframes(w.getnframes()))
    actual=array("h",(target/"core.pcm").read_bytes())
    if sys.byteorder!="little":
        reference.byteswap()
        actual.byteswap()
    count=min(len(actual),len(reference))
    assert count and abs(len(actual)-len(reference))<4*4800
    peak=0
    squares=0
    for x,y in zip(actual,reference):
        delta=abs(x-y)
        if delta>peak: peak=delta
        squares+=delta*delta
    return dict(samples=count,peak=peak,rms=math.sqrt(squares/count))

def main():
    p=argparse.ArgumentParser()
    p.add_argument("workspace",type=Path)
    p.add_argument("frontend",type=Path)
    p.add_argument("--reference",default="capture-mame-ci")
    p.add_argument("--jobs",type=int,default=4)
    args=p.parse_args()
    root=args.workspace.resolve()
    games=json.loads((root/"collection.json").read_text())
    def run(item):
        game,bios=item
        name=game["shortname"]
        target=root/args.reference/bios/name
        checkpoints=[60,180,300,360,600,900,1200]
        if name in ("cubeup","4in1","1milwhys"): checkpoints.append(1800)
        output=target/"core.raw"
        subprocess.run([args.frontend,root/"system",bios,game["path"],str(checkpoints[-1]),
                        output,"1" if name=="cubeup" else "0"],check=True,env=dict(os.environ,
                        GAMATE_PSG=str(target/"core-psg.txt"),GAMATE_PCM=str(target/"core.pcm")))
        actual=bytearray(output.read_bytes())
        reference=bytearray((target/"frames.raw").read_bytes())
        assert len(actual)==len(reference)==len(checkpoints)*96000
        reference[3::4]=bytes(len(reference)//4)
        diffs=[]
        for i,frame in enumerate(checkpoints):
            start=i*96000
            a=actual[start:start+96000]
            b=reference[start:start+96000]
            if a!=b:
                diffs.append((frame,sum(x!=y for x,y in zip(memoryview(a).cast("I"),memoryview(b).cast("I")))))
        mame_trace=(target/"mame-psg.txt").read_text().splitlines()
        core_trace=(target/"core-psg.txt").read_text().splitlines()
        common=0
        for line1,line2 in zip(mame_trace,core_trace):
            t1,*w1=map(int,line1.split()); t2,*w2=map(int,line2.split())
            if w1!=w2 or abs(t1-t2)>1: break
            common+=1
        return dict(audio=compare_audio(target),psg_common=common,psg_mame=len(mame_trace),psg_core=len(core_trace),game=game["title"],bios=bios,differences=diffs,frames=len(checkpoints))
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        rows=list(pool.map(run,((g,b) for g in games for b in ("umc","bit"))))
    (root/"comparison.json").write_text(json.dumps(rows,indent=2))
    print("Runs:",len(rows),"identical:",sum(not r["differences"] for r in rows))
    print("Audio maximum sample error:",max(r["audio"]["peak"] for r in rows))
    print("Differing checkpoints:",Counter(frame for r in rows for frame,_ in r["differences"]))

    if any(r["differences"] or r["audio"]["peak"] > 8 or
           r["psg_common"] != r["psg_core"] or r["psg_common"] != r["psg_mame"]
           for r in rows):
        raise SystemExit(1)

if __name__=="__main__":
    main()
