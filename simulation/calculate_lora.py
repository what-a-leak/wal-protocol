import math

SF = 10
IH = 0
DE = 1
CR = 4 #4/8
CRC = 1
PL = 140
BW = 125000 #Bandwidth
N_PREAMBLE = 8

r_s = BW/math.pow(2,SF)
t_s = 1/r_s
print(f"time sample: {t_s}")
t_preamble = (N_PREAMBLE+4.25)*t_s
print(f"time preamble: {t_preamble}")

numerator = (8 * PL) - (4 * SF) + 28 + (16 * CRC) - (20 * IH)
denominator = 4 * (SF - (2 * DE))
n_payload = 8 + max(math.ceil(numerator / denominator) * (CR + 4), 0)
print(f"payload size: {n_payload}")

t_payload = n_payload * t_s
print(f"payload size: {t_payload}")

t_packet = t_preamble + t_payload
print(f"packet time: {t_packet}")