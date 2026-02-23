Import("env")

try:
    with open("secrets.txt", "r") as f:
        lines = [line.strip() for line in f.readlines()]
    ssid = lines[0]
    password = lines[1]
    env.Append(BUILD_FLAGS=[
        f'-DWIFI_SSID=\\"{ssid}\\"',
        f'-DWIFI_PASS=\\"{password}\\"',
    ])
    print(f"[secrets] Loaded WiFi SSID: {ssid}")
except FileNotFoundError:
    print("[secrets] secrets.txt not found! Create it with SSID on line 1 and password on line 2.")
except IndexError:
    print("[secrets] secrets.txt format error! Need SSID on line 1 and password on line 2.")
