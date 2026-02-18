import serial
import keyboard

def main():
    while True:
        print("welcome\n")
        print("Options:\n")
        print("1. print a")
        print("2. config serial")
        print("q. quit")

        opt = input("> ").strip().lower()

        if opt == "1":
            print("\n\na\n\n")
            continue

        if opt == "2":
            com = input("enter comPort (ex: COM3): ").strip()
            baud_str = input("enter baud (ex: 9600): ").strip()

            try:
                baud = int(baud_str)
            except ValueError:
                print("baud must be a number\n")
                continue

            try:
                ser = serial.Serial(port=com, baudrate=baud, timeout=1)
            except serial.SerialException as e:
                print(f"Could not open {com}: {e}\n")
                continue

            msg = "hello\n"
            ser.write(msg.encode("utf-8"))
            ser.flush()
            print(f"Sent: {msg.strip()} to {com} at {baud} baud")

            print("Press any key to close the port...")
            keyboard.read_key()

            ser.close()
            print("Port closed.\n")
            continue

        if opt == "q":
            break

        print("Invalid option.\n")

if __name__ == "__main__":
    main()
