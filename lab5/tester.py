import math


# long calc(long nok, long first){
#     if (nok % first != 0)
#         return -1; //wrong input
#     long res = nok/first;
#     long g = 1;
#     long tmp;
#     while ( (tmp = gcd(first, res)) != g){
#         g = tmp;
#         res = nok/first * g;
#     }
#     return res;
# }

def primfacs(n):
   i = 2
   primfac = []
   while i * i <= n:
       while n % i == 0:
           primfac.append(i)
           n = n / i
       i = i + 1
   if n > 1:
       primfac.append(int(n))
   return primfac



a,b = tuple(map(int, input().split()))
# print(primfacs(25))
a_primed = primfacs(a)
b_primed = primfacs(b)
# print(b_primed)
x_primed = []

for i in b_primed:
    counted_b = b_primed.count(i)
    counted_a = a_primed.count(i)
    counted_x = x_primed.count(i)
    if counted_a == 0:
        if counted_x == 0:
            x_primed.append(tuple((i, tuple((counted_b, counted_b)))))
    elif counted_b > counted_a:
        if counted_x == 0:
            x_primed.append(tuple((i, tuple((counted_b, counted_b)))))
    elif counted_b == counted_a:
        if counted_x == 0:
            x_primed.append(tuple((i, tuple((0, counted_b)))))

# print(x_primed)

if len(x_primed) == 0:
    print(0)
else:
    count = 1
    for i in x_primed:
        count *= i[1][1] - i[1][0] + 1
    print(count)
