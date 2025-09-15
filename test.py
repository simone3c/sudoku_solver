import requests
from subprocess import run, PIPE
from csv import DictReader

levels = ["easy", "medium", "hard"]
langs = ["python", "c++ranges", "c++ranges_not_stored", "c++simple"]
bins = ["src/main.py", "", "", ""]
progs = ["python3", "bin/main_ranges.out", "bin/main_ranges_not_stored.out", "bin/main_simple.out"]

exec = {k:[p, b] for (k, p, b) in zip(langs, progs, bins)}
test_result = {k:{k:[] for k in levels} for k in langs}
repetitions = 100

def get_sudoku_online(diff):
    body = {
        "difficulty": diff, # "easy", "medium", or "hard" (defaults to "easy")
        "solution": False, # True or False (defaults to True)
        "array": False # True or False (defaults to False)
    }
    headers =  {"Content-Type":"application/json"}
    response = requests.post("https://www.youdosudoku.com/api/", json=body, headers=headers)
    if response.status_code == 200:
        data = response.json()
        print(data)
        return data['puzzle']
    else:
        print(f"Error: {response.status_code}")


def run_test(sudoku: str, diff: str, lang: str):
    ret = 0

    prog, bin, = exec[lang]
    args = [prog, bin, sudoku, str(repetitions)]
    if bin == "":
        del args[1]
        
    ret = run(args, stdout=PIPE)

    test_result[lang][diff].append(ret.stdout.decode().strip())


puzzles = dict()

with open("sudoku.csv", "r") as f:
    puzzles = DictReader(f)

    for row in puzzles:
        for lang in langs[1:]:
            run_test(row["puzzle"], row["level"], lang)



for (k, v) in test_result.items():
    print(f"{k}:")
    for (lv, t) in v.items():
        for (i, tt) in enumerate(t):
            print(f"\t{lv:>6} ({i + 1}): {tt:>10} ns")
    print("")