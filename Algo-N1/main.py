import sys

def read_input_file(file_path):
    test_cases = []

    with open(file_path, 'r') as file:
        for line in file:
            parts = line.strip().split()

            N = int(parts[0])
            SGold = int(parts[1])
            VGold = int(parts[2])
            SSilver = int(parts[3])
            VSilver = int(parts[4])

            test_cases.append((N, SGold, VGold, SSilver, VSilver))

    return test_cases


def write_output_file(file_path, results):
    first = True
    with open(file_path, 'w') as file:
        for result in results:
            if first:
                first = False
                file.write(str(result))
            else:
                file.write('\n' + str(result))


def solve(test_cases):
    results = []
    for test_case in test_cases:
        result = solve_one(test_case)

        results.append(result)
        print(result)

    return results


def solve_one(test_case):
    N, SGold, VGold, SSilver, VSilver = test_case

    if (N < SGold) & (N < SSilver):
        return 0

    gold1 = N // SGold
    silver1 = (N - gold1 * SGold) // SSilver

    silver2 = N // SSilver
    gold2 = (N - silver2 * SSilver) // SGold

    result1 = gold1 * VGold + silver1 * VSilver
    result2 = gold2 * VGold + silver2 * VSilver

    if result1 > result2:
        return result1
    else:
        return result2


def check_files(file_path1, file_path2):
    line_count = 0
    returning_val = True
    with open(file_path1, 'r') as file1:
        with open(file_path2, 'r') as file2:
            for line1, line2 in zip(file1, file2):
                line_count += 1
                if line1 != line2:
                    print(line_count, line1, line2)
                    returning_val = False

    return returning_val


if __name__ == '__main__':
    if len(sys.argv) > 1:
        first_argument = sys.argv[1]
    else:
        print("No argument provided.")
        exit(1)

    file_path = str(first_argument)
    test_cases = read_input_file(file_path)
    results = solve(test_cases)

    #write_output_file('output.txt', results)
    #print('Checking files...')
    #check_files('output.txt', 'resitev_testni_primer.txt')


