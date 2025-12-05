# LifeBoat
Status: Non functioning, work-in-progress.
A project by [SnoopSqueak](https://snoopsqueak.com).

The goal is to create a peer-to-peer self-hosted secure chat app.

It is small and simple for maximum accessibility.

The C code in this project is intended to be beginner-friendly.

### Programming style:
Inspired by this [Linux kernel coding style guide](https://www.kernel.org/doc/html/latest/process/coding-style.html).

Philosophy:
- Divide a big, unmanageable thing into manageable pieces we can focus on.
- Short and simple pieces: 3-7 things per block for human manageability.
- No one will be happy: the best way forward is somewhere between fast and good.

Readability:
- Limit every line of code to 79 characters at most.
- Use 8 spaces of indentation to discourage nesting and long lines.
- Aim for concise variable and function names. Abbreviate to fit line limits.

Performance:
- Pass by reference for any data larger than "double" type in size.
- Avoid redundant memory allocation. Make careful use of pointers.

Clarity:
- Avoid global variables.
- Avoid unnecessary use of typedef.
- Avoid enums.


Nothing is set in stone. Thank you for reading.