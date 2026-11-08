import math, random, struct
import numpy as np
from PIL import Image
W,H=240,160
rnd=random.Random(11)
img=np.zeros((H,W,3),float)
cx,cy=120,70
for y in range(H):
    for x in range(W):
        t=y/H
        base=np.array([40,26,90])*(1-t)+np.array([14,10,46])*t
        d=math.hypot(x-cx,(y-cy)*1.2)
        glow=max(0,1-d/110)**1.6
        ang=math.atan2(y-cy,x-cx)
        ray=0.5+0.5*math.cos(ang*12)
        ray=1 if ray>0.78 else 0
        col=base+np.array([70,50,110])*glow*0.8+np.array([22,16,40])*ray*max(0,1-d/190)
        img[y,x]=col
im=Image.fromarray(np.clip(img,0,255).astype(np.uint8))
# stars
px=im.load()
for _ in range(34):
    x=rnd.randint(2,W-3); y=rnd.randint(2,100)
    px[x,y]=(230,230,255)
    if rnd.random()<0.3:
        for dx,dy in((1,0),(-1,0),(0,1),(0,-1)): px[x+dx,y+dy]=(150,150,220)
q=im.quantize(colors=16,method=Image.Quantize.MEDIANCUT,dither=Image.Dither.NONE)
pal=q.getpalette()[:48]
a=np.array(q)
tiles={}; order=[]; tmap=[]
for ty in range(H//8):
    for tx in range(W//8):
        blk=a[ty*8:ty*8+8,tx*8:tx*8+8]
        key=blk.tobytes()
        if key not in tiles:
            tiles[key]=len(order); order.append(blk)
        tmap.append(tiles[key])
print('unique tiles',len(order))
out=bytearray()
for blk in order:
    for y in range(8):
        for x in range(0,8,2):
            out.append(int(blk[y,x])|(int(blk[y,x+1])<<4))
open('/home/claude/pokefirered/graphics/zumo_gacha/bg.4bpp','wb').write(out)
open('/home/claude/pokefirered/graphics/zumo_gacha/bg_map.bin','wb').write(struct.pack('<%dH'%len(tmap),*tmap))
gp=bytearray()
for i in range(16):
    r,g,b=pal[i*3:i*3+3]
    gp+=struct.pack('<H',(r>>3)|((g>>3)<<5)|((b>>3)<<10))
open('/home/claude/pokefirered/graphics/zumo_gacha/bg.gbapal','wb').write(gp)
q.convert('RGB').resize((480,320),Image.NEAREST).save('/home/claude/mock/bg_preview.png')
