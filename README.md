<p align="center">
    <img src="https://game.42sp.org.br/static/assets/achievements/ft_irce.png">
</p>

<p align="center">
    <img src="https://img.shields.io/badge/OS-Linux-blue" alt="OS">
    <img src="https://img.shields.io/badge/Language-C%2B%2B98-orange.svg" alt="Language">
    <img src="https://img.shields.io/badge/Grade-100%2F100-brightgreen.svg" alt="Grade">
    <img src="https://img.shields.io/badge/Status-Completed-brightgreen.svg" alt="Status">
</p>

# 42 IRC
A simple IRC server that adheres to the following RFCs:

- [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2180](https://datatracker.ietf.org/doc/html/rfc2180)
- [RFC 2181](https://datatracker.ietf.org/doc/html/rfc2181)
- [RFC 2182](https://datatracker.ietf.org/doc/html/rfc2182)

> [!NOTE]
> This server prioritizes core IRC functionalities, providing a platform for users to connect, chat, and join channels. It does not currently support features related to inter-server connections (IRC networks).

## Features
- Compliant with the listed RFCs for standardized and reliable communication
- Supports basic user management (registration, nicknames, modes)
- Enables channel creation, joining, and private messaging
- Offers user and channel operator functionalities

## Getting Started
### Requirements
The project requires the following tools:

* Clang 12
* GNU Make
* libcxxClang

### Building the Server

There are two ways to build the server:

**With Nix:**

Run the following command:

```bash
$ nix build .#ircserv
```

**Without Nix:**

Navigate to the root directory of the repository and run:

```bash
$ make -C ./ft-irc
```

### Running the Server
The server requires two positional arguments:

- `PORT`: The port on which the server will listen (e.g., 6667)
- `PASSWORD`: The password required for users to connect to the server

**Example:**

```bash
$ ./ircserv 6667 strongpassword
Started listening on 0.0.0.0:6667.
```

## Authors
- [Vinicius Oliveira (vgoncalv)](https://github.com/vinicius507)
- [Lucas Pires (lpires-n)](https://github.com/iLucasPires)
- [Marcelo (marcrodr)](https://github.com/meritissimo1)
