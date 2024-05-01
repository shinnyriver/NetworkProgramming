import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# 설정값
nodes = 50
frame_size = 20
total_slots = 10_000_000
p_values = np.power(10, np.arange(-2, 0, 0.2))

# 결과를 저장할 데이터프레임
# 1-persistent CSMA/CD 시뮬레이션 (pd.concat 사용)
results_1_persistent_updated = pd.DataFrame(columns=['p', 'utilization'])

for p in p_values:
    slots = np.zeros(total_slots)  # 각 슬롯의 상태를 저장 (0: idle, 1: busy)
    successful_transmissions = 0  # 성공적인 전송의 수

    for _ in range(nodes):
        if np.random.rand() < p:  # p의 확률로 프레임 전송 요청
            start_slot = np.random.randint(0, total_slots - frame_size)
            # 채널이 idle할 때까지 대기
            while np.any(slots[start_slot:start_slot + frame_size] == 1):
                start_slot += 1
                if start_slot >= total_slots - frame_size:
                    break
            # 채널이 idle이면 전송 시작
            if start_slot < total_slots - frame_size:
                slots[start_slot:start_slot + frame_size] = 1
                successful_transmissions += frame_size

    # Utilization 계산
    utilization = successful_transmissions / total_slots
    new_row = pd.DataFrame({'p': [p], 'utilization': [utilization]})
    results_1_persistent_updated = pd.concat([results_1_persistent_updated, new_row], ignore_index=True)

# 그래프 그리기
plt.figure(figsize=(10, 6))
plt.plot(results_1_persistent_updated['p'], results_1_persistent_updated['utilization'], marker='o', linestyle='-', color='r')
plt.title('CSMA/CD 1-persistent Utilization vs. p (Updated)')
plt.xlabel('p (log scale)')
plt.ylabel('Utilization')
plt.xscale('log')
plt.grid(True, which="both", ls="--")
plt.show(), results_1_persistent_updated
