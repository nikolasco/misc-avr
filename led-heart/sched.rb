# 3 bits
# length of 7
# bit schedule: 2120212
#   by bit:
#     2: 0,2,4,6
#     1: 1,5
#     0: 3

# 4 bits
# length of 15
# bit schedule: 323132303231323
#   by bit:
#     3: 0,2,4,6,8,10,12,14
#     2: 1,5,9,13
#     1: 3,11
#     0: 7

# note that the halves are identical
#   true recursively!
# note that if you take the first half of the 4-bit schedule and subtract 1
# from each bit-position you get the 3 bit schedule

# THEORY: position of first 0 from right is almost the bit position.
#  just subtract it from largest bit position (1 - number of bits) to get actual position
# example:
# 9 in binary is 1001
#   not even, shift right
#   100
# took 1 step
# 4 bit schedule, so 4 - 1 - 1 = 2

UPPER_LIM_POW = 9
MAX_BIT_POS = UPPER_LIM_POW - 1
puts 0.upto(2**UPPER_LIM_POW - 2).map { |t|
  b = MAX_BIT_POS
  until t.even?
    t >>= 1
    b -= 1
  end
  b
}.join(', ')
