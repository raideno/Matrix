def permut(a, b):
    c = a
    a = b
    b = c
    
    return a, b
    
a = 0
b = 1

print(f"a={a} b={b}")

a, b = permut(a, b)

print(f"a={a} b={b}")

