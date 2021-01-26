# BDSH

A simple exploration of C programming an creating something bigger than a 
"Hello World" program that actually has some real world applications.

### Design Goals

The goal with making this was to try making one of the most used pieces of
software in the modern UNIX world. My idea for this project was to find out what
the basis of a UNIX system is along with leaning more about the C language.

### Choices I Made

I have made several design choices during the course of this project for very
specific reasons. One of those was the intentional omission of many C library
functions that would have made development easier. This was in an attempt to
give myself a challenge as well as giving me a more complete understanding of
the C language and what is possible with pointers.

### Future Plans

Now that the shell is in a mostly functional state, I won't be doing many
updates. I will attempt to add more builtins along with a more complete command
parser. 

Items I wish to add stand as follows:
* Arrow key support
* More builtins
	* Bourne shell compatability?
		* Bourne again shell compatability?
			* Even more?
* Complete command parser
* Ability to read `.sh` files
* Tab completion

### Builtins
I created a simple set of builtins to get the basic functionality running, but
it would be ideal to add some more and flesh out the capabilities of the
already existing builtins.

Current:
* `cd <dir>` - Changes current working directory, accepts one argument. If no
argument is suppled, the shell changes to the user's home dirrectory.
* `exit <code>` - Exits the shell, accepts one argument as the code to exit.
If no code is supplied, the shell with exit with code `0`.
* `help <command>` - Gives help with the specified command. If no argument is
supplied, general help will be shown.

Planned:
* `: [args]`
* `. <file> [args]`
* `$<env>` Expansion
* `export <env>`/`let <env>`
* `unset <env>`
