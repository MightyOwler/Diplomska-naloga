class FreeGroupElement:
    def __init__(self, word):
        self.word = self.simplify(word)

    @staticmethod
    def simplify(word):
        stack = []
        for gen in word:
            if stack and stack[-1] == gen + "^-1":
                stack.pop()
            elif stack and stack[-1] + "^-1" == gen:
                stack.pop()
            elif (
                stack
                and stack[-1][0] == gen[0]
                and "^" not in stack[-1]
                and "^" not in gen
            ):
                stack.append(f"{gen[0]}^{2}")
            elif stack and stack[-1][0] == gen[0] and "^" in stack[-1] and "^" in gen:
                base, power1 = stack[-1].split("^")
                _, power2 = gen.split("^")
                new_power = int(power1) + int(power2)
                stack.pop()
                if new_power != 0:
                    stack.append(f"{base}^{new_power}")
            elif stack and stack[-1][0] == gen[0] and "^" in stack[-1]:
                base, power1 = stack[-1].split("^")
                new_power = (
                    int(power1) + 1 if "^" not in gen else int(power1) + int(gen[2:])
                )
                stack.pop()
                if new_power != 0:
                    stack.append(f"{base}^{new_power}")
            elif stack and stack[-1][0] == gen[0] and "^" in gen:
                new_power = (
                    1 + int(gen[2:])
                    if "^" not in stack[-1]
                    else int(stack[-1][2:]) + int(gen[2:])
                )
                stack.pop()
                if new_power != 0:
                    stack.append(f"{gen[0]}^{new_power}")
            else:
                stack.append(gen)
        return stack

    def __mul__(self, other):
        return FreeGroupElement(self.word + other.word)

    def inverse(self):
        inv_word = []
        for gen in reversed(self.word):
            if "^" in gen:
                base, power = gen.split("^")
                inv_word.append(f"{base}^{-int(power)}")
            else:
                inv_word.append(gen + "^-1")
        return FreeGroupElement(inv_word)

    def commutator(self, other):
        return self * other * self.inverse() * other.inverse()

    def __len__(self):
        length = 0
        for gen in self.word:
            if "^" in gen:
                _, power = gen.split("^")
                length += abs(int(power))
            else:
                length += 1
        return length

    def __repr__(self):
        result = []
        for gen in self.word:
            if "^" in gen:
                base, power = gen.split("^")
                if power == "-1":
                    result.append(base + "⁻¹")
                elif power == "2":
                    result.append(base + "²")
                else:
                    result.append(gen)
            else:
                result.append(gen)
        return "".join(result).replace("^-1", "⁻¹")


def generate_sequences(n):
    a_0 = FreeGroupElement(["a"])
    b_0 = FreeGroupElement(["b"])
    a_seq = [a_0]
    b_seq = [b_0]

    for i in range(1, n + 1):
        a_n = b_seq[i - 1].inverse().commutator(a_seq[i - 1])
        b_n = a_seq[i - 1].commutator(b_seq[i - 1])
        a_seq.append(a_n)
        b_seq.append(b_n)

    return a_seq, b_seq


# Example usage
n = 5  # Change this value to compute different terms in the sequence
a_seq, b_seq = generate_sequences(n)

for i in range(n + 1):
    # print(f"a_{i} = {a_seq[i]}, length a_{i} = {len(a_seq[i])}")
    # print(f"b_{i} = {b_seq[i]}, length b_{i} = {len(b_seq[i])}")
    print(f"length a_{i} = {len(a_seq[i])}")
    print(f"length b_{i} = {len(b_seq[i])}")


print(len(a_seq[3]), len(b_seq[3]))
