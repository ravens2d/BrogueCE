import subprocess
import time


def read_state(process):
    state = ''
    while process.poll() is None:
        line = process.stdout.readline()
        if line.strip() == "<state>":
            continue
        if line.strip() == "</state>":
            break
        state += line
    return state


if __name__ == '__main__':
    try:
        process = subprocess.Popen(["./brogue", "-n", "--gym-mode"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True, bufsize=1)
        print(f"Process started with PID: {process.pid}")

        while process.poll() is None:
            state = read_state(process)

            print(state)

            process.stdin.writelines(["up\n"])
            process.stdin.flush()
            time.sleep(1)

    finally:
        process.terminate()
