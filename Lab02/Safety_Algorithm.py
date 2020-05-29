import sys

def print_safe_sequence(allocations, needs, availables, sequence):
    global process_count, instance_count
    if(len(sequence) == process_count):
        print("시작", end=" => ")
        for process_num in sequence:
            print("process{}".format(process_num), end=" => ")
        print("종료")
    else:
        for i in range(process_count):
            if i not in sequence:
                is_safe = True
                for j in range(instance_count):
                    if availables[j] < needs[i][j]:
                        is_safe = False
                        break
                if is_safe:
                    new_availables = availables[:]
                    new_sequence = sequence[:]
                    for j in range(instance_count):
                        new_availables[j] = availables[j] + allocations[i][j]
                    new_sequence.append(i)

                    print_safe_sequence(allocations, needs, new_availables, new_sequence)

print("[입력]")                  
print("\n====== Process 갯수 입력 ====== ")
process_count = int(input("process 갯수를 입력해주세요(2~10) : "))
if process_count < 2 or process_count > 10:
    print("[Error] process 갯수를 잘못 입력하셨습니다. 프로그램을 종료합니다.")
    sys.exit()

print("\n====== Resource 갯수 입력 ======")
resource_count = int(input("resource 갯수를 입력해주세요(2~5) : "))
if resource_count < 2 or resource_count > 5:
    print("[Error] resource 갯수를 잘못 입력하셨습니다. 프로그램을 종료합니다.")
    sys.exit()

print("====== Instance 입력 ====== ")
instances = [] 
instance_count = 0
while(instance_count<resource_count):
    instance = int(input("{}번째 자원의 instance 갯수를 입력해주세요(0~99) : ".format(instance_count + 1)))
    if instance == 0:
        break
    if instance < 0 or instance > 99:
        print("[Error] instance 갯수를 잘못 입력하셨습니다. 프로그램을 종료합니다.")
        sys.exit()
    instances.append(instance)
    instance_count += 1

if instance_count < 2:
    print("[Error] 자원 종류는 2개 이상만 가능합니다. 프로그램을 종료합니다.")
    sys.exit()

print("\n====== Allocation 입력 ====== ")
allocations = []
availables = instances[:]
for i in range(process_count):
    allocation = input("process{}의 자원을 할당해주세요(띄어쓰기로 구분 ex) 0 1 2 ...) : ".format(i)).split(" ")
    if len(allocation) != instance_count:
        print("[Error] 자원 종류와 입력된 자원의 갯수가 같지 않습니다. 프로그램을 종료합니다.")
        sys.exit()
    for j in range(instance_count):
        allocation[j] = int(allocation[j])
        availables[j] = availables[j] - allocation[j]
        if allocation[j] < 0 or allocation[j] > instances[j] or availables[j] < 0:
            print("[Error] 자원 입력이 잘못되었습니다. 프로그램을 종료합니다.")
            sys.exit()
    allocations.append(allocation)

print("\n====== Max 입력 ====== ")
maxes = []
needs = []
for i in range(process_count):
    max_row = input("process{}의 자원의 최대치를 입력해주세요(띄어쓰기로 구분 ex) 0 1 2 ...) : ".format(i)).split(" ")
    need = []
    if len(max_row) != instance_count:
        print("[Error] 자원 종류와 입력된 자원의 개수가 같지 않습니다. 프로그램을 종료합니다.")
        sys.exit()
    for j in range(instance_count):
        max_row[j] = int(max_row[j])
        if max_row[j] < 0 or max_row[j] > instances[j] or max_row[j] < allocations[i][j]:
            print("[Error] 자원 입력이 잘못되었습니다. 프로그램을 종료합니다.")
            sys.exit()
        need.append(max_row[j] - allocations[i][j])
    maxes.append(max_row)
    needs.append(need)

print("\n[출력]")
print("====== Instance ====== ")
print("Instances : ", end="")
for i in range(instance_count):
    print(instances[i], end=" ")
print("")

print("Available : ", end="")
for i in range(instance_count):
    print(availables[i], end=" ")
print("")

print("\n====== Allocation ====== ")
for i in range(process_count):
    print("process{} : ".format(i), end="")
    for j in range(instance_count):
        print(allocations[i][j], end=" ")
    print("")

print("\n====== Max ====== ")
for i in range(process_count):
    print("process{} : ".format(i), end="")
    for j in range(instance_count):
        print(maxes[i][j], end=" ")
    print("")

print("\n====== Need ======")
for i in range(process_count):
    print("process{} : ".format(i), end="")
    for j in range(instance_count):
        print(needs[i][j], end=" ")
    print("")

print("\n====== 가능한 Safety sequence 종류 ======")
print_safe_sequence(allocations, needs, availables, [])
