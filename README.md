# Diceware

## About

This is a simple diceware passphrase generator using the OS' random source, you can:

- Choose how many words you want
- Choose a dictionary

It comes with an English dictionary with 7776 unique words.

## Why use it?

![xkcd](https://imgs.xkcd.com/comics/password_strength.png)

Also: https://security.stackexchange.com/questions/62832/is-the-oft-cited-xkcd-scheme-no-longer-good-advice

## How to build it

It uses the C standard library, so it should be easy to compile on any UNIX like OS providing `/dev/urandom`. Follow the following steps:

- Install `make`
- Install `gcc`
- Run `make` on the project directory

## How to run

The usage is self-explanatory, just make sure the dictionary is on the same directory as the executable and run from the command line.

## File format

There's a table in front of the words with offsets into the file. 
