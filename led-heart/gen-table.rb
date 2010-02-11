# this script generates the lookup table that maps between lightness
# and luminance

# this function is Pauli's generalization of the 1976 CIELAB lightness function
# (scaled to output 0-1 instead of 0-100)
# http://en.wikipedia.org/wiki/Lightness_%28color%29
L_TIP_POINT = (6/29.0)**3
def lightness(u)
  l = (u <= L_TIP_POINT) ? ((841.0/108)*u + 4.0/29) : u**(1.0/3)
  (l*116 - 16)/100
end

# this is meant to be the inverse of lightness()
V_TIP_POINT = 6.0/29
def value(l)
  l = (l*100 + 16)/116.0
  l < V_TIP_POINT ? ((l-(4.0/29))*(108.0/841)) : l**3
end

IN_UPPER_LIM = 2**8 - 1
OUT_UPPER_LIM = 2**9 - 1
puts 0.upto(IN_UPPER_LIM).map{ |l|
  l *= 1.0/IN_UPPER_LIM;
  (value(l)*OUT_UPPER_LIM).round
}.join(', ')

