# NFC URL

A Flipper Zero application that generates and emulates NFC tags that point to URLs.

Features:
- Generate http(s), (s)ftp, mailto, etc. URLs
- Save a list URLs for later use
- Export to a Flipper NFC Tag file (.nfc)

## Building & Installation

NFC URL will have GitHub releases and be available (hopefully) on the Flipper App Catalog soon. Until then, build it yourself:

1. Clone this repository.
2. Install [uFBT](https://github.com/flipperdevices/flipperzero-ufbt)
3. Plug in your Flipper and run `ufbt launch` from the project directory.

## Usage

1. Press "New" to create a new NFC Tag
<img src="https://cdn.discordapp.com/attachments/941082548900073472/1252726481851908167/flpr-2024-06-18-13-47-35.png?ex=667343f3&is=6671f273&hm=848211a02a7910ac55572ea728903feac7ad50e4af1a5b98feb8d8a90a69abff&" width="400px" />
2. Enter your URL
<img src="./screenshots/prefix.png" width="200px" /> <img src="./screenshots/enter-url.png" width="400px" />
3. Emulate your NFC tag
<img src="./screenshots/created.png" width="400px" /> <img src="./screenshots/emulate.png" width="400px" />
4. Save/export the tag to use later
<img src="./screenshots/options.png" width="400px" /> <img src="./screenshots/savedlist.png" width="200px" />