# Chewie
Fork of the Fermilab's test beam analysis software (non-github).

### Requirements for installation:

To install Monicelli and Chewie, these are the current dependencies:

- ROOT 6.06.06
- Qt/QtCreator 5.7.1
- Boost default
- XercesC 3.1.3

Check out [setup-6.sh](./setup-6.sh) for where I have these packages on lnx231. Qt 5.7.1 was already installed, as well as Boost. I custom-
installed ROOT and XercesC, but your mileage may vary.

### Installing Chewie

Once all the dependencies have been installed, git clone this repository and adjust the environment variables in [setup-6.sh](./setup-6.sh)
to match your set-up. After that, source the file and issue qmake, followed by make. This should build Chewie successfully.

### Monicelli and Chewie tutorials

You can find a tutorial session and slides on how to use Monicelli and Chewie here:

https://indico.cern.ch/event/705870/
