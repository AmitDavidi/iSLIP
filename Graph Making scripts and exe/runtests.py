import subprocess
seed = 45
T = 10000
commands = [
    f"tr_gen.exe 16 {T} {seed} 0.6 |     islip.exe 16 1 1 > out_script1.txt",
    f"tr_gen.exe 16 {T} {seed} 0.7 |     islip.exe 16 1 2 > out_script2.txt",
    f"tr_gen.exe 16 {T} {seed} 0.75 |    islip.exe 16 1 3 > out_script3.txt",
    f"tr_gen.exe 16 {T} {seed} 0.8 |     islip.exe 16 1 4 > out_script4.txt",
    f"tr_gen.exe 16 {T} {seed} 0.85 |    islip.exe 16 1 5 > out_script5.txt",
    f"tr_gen.exe 16 {T} {seed} 0.9 |     islip.exe 16 1 6 > out_script6.txt",
    f"tr_gen.exe 16 {T} {seed} 0.95 |    islip.exe 16 1 7 > out_script7.txt",
    f"tr_gen.exe 16 {T} {seed} 0.99 |    islip.exe 16 1 8 > out_script8.txt",
    f"tr_gen.exe 16 {T} {seed} 0.9 -d |  islip.exe 16 1 9 > out_script9.txt",
    f"tr_gen.exe 16 {T} {seed} 0.95 -d | islip.exe 16 1 10 > out_script10.txt",
    f"tr_gen.exe 16 {T} {seed} 0.99 -d | islip.exe 16 1 11 > out_script11.txt",
    f"tr_gen.exe 16 {T} {seed} 0.9 |     islip.exe 16 3 12 > out_script12.txt",
    f"tr_gen.exe 16 {T} {seed} 0.95 |    islip.exe 16 3 13 > out_script13.txt",
    f"tr_gen.exe 16 {T} {seed} 0.99 |    islip.exe 16 3 14 > out_script14.txt",
    f"tr_gen.exe 32 {T} {seed} 0.9 |     islip.exe 32 1 15 > out_script15.txt",
    f"tr_gen.exe 32 {T} {seed} 0.95 |    islip.exe 32 1 16 > out_script16.txt",
    f"tr_gen.exe 32 {T} {seed} 0.99 |    islip.exe 32 1 17 > out_script17.txt"
]

for command in commands:
    subprocess.run(command, shell=True, check=True)
