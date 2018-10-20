import sys

Csize = input()
bsize = int(Csize)

#print(type(bsize))
ans = 0

bits = [0] * bsize

temp_bits = bits

counter = 0

while ( ans < bsize ):

    temp_str =  "".join([str(x) for x in temp_bits] )
    print('Q ', end='')
    print (" ".join(temp_str))
    sys.stdout.flush()

    

    temp_class = input()
    temp_ans = int(temp_class)

    if(temp_ans > ans):

        ans = temp_ans

        bits = temp_bits
        if (ans == bsize):
                temp_str =  "".join([str(x) for x in temp_bits] )
                print('A ', end='')
                print (" ".join(temp_str))
                sys.stdout.flush()
                sys.exit()

    if(temp_ans < ans):
        temp_bits[counter-1] = bits[counter-1] -1

    temp_bits[counter] = bits[counter] + 1

    counter += 1




temp_str =  "".join([str(x) for x in temp_bits] )
print('A ', end='')
print (" ".join(temp_str))
sys.stdout.flush()




