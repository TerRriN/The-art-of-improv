def v1_prime_counter():
    i = 2
    prime_numbers = [2]

    while i < 100:
        for x in prime_numbers:
            if((i % x) == 0):
                break
            
            if(i == len(prime_numbers)):
                prime_numbers.append(i)

        print(i)
        i += 1
    print(prime_numbers)

v1_prime_counter()