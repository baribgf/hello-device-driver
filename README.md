# A simple `Hello, $(name)!` pseudo-device driver

This kernel module implements a pseudo-device driver,
which offers the user two essential operations on the 
device: he can write to it specifying a name for greeting,
then he can read from it to get a greeting message in the
form like Hello, `name`!

This device driver module does no real useful thing other
than illustrating some of the working mechanisms of linux
kernel drivers, and offering some enjoyable activity for
linux enthusiasts.

## Usage:

1. Build the driver module using:
```bash
make build
```

2. Load the module into kernel space:
```bash
make load
```

3. Create the device file:
```bash
make dev
```

4. Play with the device:
```
cat ./hello  # Outputs: "Hello, World!" when executed for the first time
echo -n "Mom" > ./hello
cat ./hello  # Outputs: "Hello, Mom!"
```

Written by: Abdelbari BOUGOFFA \
On: Oct 15th, 2025
