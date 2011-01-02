#from scipy.interpolate import interp1d, linspace
import numpy
import scipy.interpolate


xn = [ -4500, -4000, -3500, -3000, -2500, -2000, -1500, -1000, -500,     0]
rn = [   113,   121,   132,   141,   150,   161,   172,   185,   198,  216]
gn = [   171,   178,   185,   193,   201,   210,   219,   227,   236,  242]
bn = [   216,   222,   227,   234,   240,   247,   251,   255,   255,  254]

xp = [   0, 300, 600,  900, 1200, 1500, 1800, 2100, 2400, 2700, 3000, 3300, 3600, 3900, 4200, 4500, 4800, 5100, 5400]
rp = [ 172, 148, 168,  189,  209,  225,  239,  232,  222,  211,  202,  195,  185,  170,  172,  186,  202,  224,  245]
gp = [ 208, 191, 198,  204,  215,  228,  235,  225,  214,  202,  185,  167,  152,  135,  154,  174,  195,  222,  244]
bp = [ 165, 139, 143,  150,  171,  181,  192,  182,  163,  157,  130,  107,   90,   83,  124,  154,  184,  216,  242]

xwh = [   0,  50, 200,  600, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 7800]
rwh = [  77,  88, 101,  147,  204,  206,  206,  204,  213,  230,  241,  255]
gwh = [ 141, 162, 175,  183,  190,  169,  179,  194,  213,  230,  241,  255]
bwh = [ 114, 129, 138,  147,  163,  142,  162,  192,  213,  230,  241,  255]

xi = [   0,  50, 200,  600, 1000, 2000, 3000, 4000]
ri = [ 128, 145, 153,  169,  188,  210,  233,  234]
gi = [ 151, 164, 178,  188,  204,  216,  239,  240]
bi = [ 183, 196, 208,  221,  229,  238,  253,  252]



fnr = scipy.interpolate.interp1d(xn, rn, kind='cubic')
fng = scipy.interpolate.interp1d(xn, gn, kind='cubic')
fnb = scipy.interpolate.interp1d(xn, bn, kind='cubic')

fpr = scipy.interpolate.interp1d(xp, rp, kind='cubic')
fpg = scipy.interpolate.interp1d(xp, gp, kind='cubic')
fpb = scipy.interpolate.interp1d(xp, bp, kind='cubic')

fwhr = scipy.interpolate.interp1d(xwh, rwh, kind='cubic')
fwhg = scipy.interpolate.interp1d(xwh, gwh, kind='cubic')
fwhb = scipy.interpolate.interp1d(xwh, bwh, kind='cubic')

fir = scipy.interpolate.interp1d(xi, ri, kind='cubic')
fig = scipy.interpolate.interp1d(xi, gi, kind='cubic')
fib = scipy.interpolate.interp1d(xi, bi, kind='cubic')





xxn = numpy.linspace(-4500, 0, 101)
print xxn




#xx = numpy.arange(-5000, 7250, 50)
#print xx

i=0
a="int rn[101] =  {    "
for alt in xxn :
    vnr= int(round(fnr(alt)))

    a=a+"%i," % (vnr)
    i=i+1

print a

i=0
a="int gn[101] =  {    "
for alt in xxn :
    vng= int(round(fng(alt)))

    a=a+"%i," % (vng)
    i=i+1

print a

i=0
a="int bn[101] =  {    "
for alt in xxn :
    vnb= int(round(fnb(alt)))

    a=a+"%i," % (vnb)
    i=i+1

print a

print "positif"
xxp = numpy.linspace(0, 5400, 121)
print xxp

i=0
a="int rp[121] =  {    "
for alt in xxp :
    vpr= int(round(fpr(alt)))

    a=a+"%i," % (vpr)
    i=i+1

print a

i=0
a="int gp[121] =  {    "
for alt in xxp :
    vpg= int(round(fpg(alt)))

    a=a+"%i," % (vpg)
    i=i+1

print a

i=0
a="int bp[121] =  {    "
for alt in xxp :
    vpb= int(round(fpb(alt)))

    a=a+"%i," % (vpb)
    i=i+1

print a

print "positif"
xxp = numpy.linspace(0, 4800, 121)
print xxp

i=0
a="int rwhp[121] =  {    "
for alt in xxp :
    vwhr= int(round(fwhr(alt)))

    a=a+"%i," % (vwhr)
    i=i+1

print a

i=0
a="int gwhp[121] =  {    "
for alt in xxp :
    vwhg= int(round(fwhg(alt)))

    a=a+"%i," % (vwhg)
    i=i+1

print a

i=0
a="int bwhp[121] =  {    "
for alt in xxp :
    vwhb= int(round(fwhb(alt)))

    a=a+"%i," % (vwhb)
    i=i+1

print a


print "Ice"
xxp = numpy.linspace(0, 4000, 17)
print xxp

i=0
a="int ri[16] =  {    "
for alt in xxp :
    vir= int(round(fir(alt)))

    a=a+"%i," % (vir)
    i=i+1

print a

i=0
a="int gi[16] =  {    "
for alt in xxp :
    vig= int(round(fig(alt)))

    a=a+"%i," % (vig)
    i=i+1

print a

i=0
a="int bip[16] =  {    "
for alt in xxp :
    vib= int(round(fib(alt)))

    a=a+"%i," % (vib)
    i=i+1

print a
