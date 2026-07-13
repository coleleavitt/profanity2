# profanity2

Profanity is a high performance (probably the fastest!) vanity address generator for Ethereum. Create cool customized addresses that you never realized you needed! Recieve Ether in style! Wow!

![Screenshot](/img/screenshot.png?raw=true "Wow! That's a lot of zeros!")

# Important to know

A previous version of this project has a known critical issue due to a bad source of randomness. The issue enables attackers to recover private key from public key: https://blog.1inch.io/a-vulnerability-disclosed-in-profanity-an-ethereum-vanity-address-tool

This project "profanity2" was forked from the original project and modified to guarantee **safety by design**. This means source code of this project do not require any audits, but still guarantee safe usage.

Project "profanity2" is not generating key anymore, instead it adjusts user-provided public key until desired vanity address will be discovered. Users provide seed public key in form of 128-symbol hex string with `-z` parameter flag. Resulting private key should be used to be added to seed private key to achieve final private key of the desired vanity address (private keys are just 256-bit numbers). Running "profanity2" can even be outsourced to someone completely unreliable - it is still safe by design.

## Getting public key for mandatory `-z` parameter

Generate private key and public key via openssl in terminal (remove prefix "04" from public key):
```bash
$ openssl ecparam -genkey -name secp256k1 -text -noout -outform DER | xxd -p -c 1000 | sed 's/41534e31204f49443a20736563703235366b310a30740201010420/Private Key: /' | sed 's/a00706052b8104000aa144034200/\'$'\nPublic Key: /'
```

Derive public key from existing private key via openssl in terminal (remove prefix "04" from public key):
```bash
$ openssl ec -inform DER -text -noout -in <(cat <(echo -n "302e0201010420") <(echo -n "PRIVATE_KEY_HEX") <(echo -n "a00706052b8104000a") | xxd -r -p) 2>/dev/null | tail -6 | head -5 | sed 's/[ :]//g' | tr -d '\n' && echo
```

## Adding private keys (never use online calculators!)

### Terminal:

Use private keys as 64-symbol hexadecimal string WITHOUT `0x` prefix:
```bash
(echo 'ibase=16;obase=10' && (echo '(PRIVATE_KEY_A + PRIVATE_KEY_B) % FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F' | tr '[:lower:]' '[:upper:]')) | bc
```

### Python

Use private keys as 64-symbol hexadecimal string WITH `0x` prefix:
```bash
$ python3
>>> hex((PRIVATE_KEY_A + PRIVATE_KEY_B) % 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F)
```

# 6-way GLV/negation search (this fork)

This fork checks **six** candidate addresses per elliptic-curve point instead of
one, using the secp256k1 GLV endomorphism (`(x,y)`, `(βx,y)`, `(β²x,y)`) and
point negation (each of those with `-y`). The five extra candidates cost only a
couple of field multiplications plus one keccak each, so effective throughput
(candidate addresses per second, which is what `Total: … MH/s` now reports)
rises by roughly **1.8–1.9x**. The idea is the same symmetry trick VanitySearch
uses for Bitcoin; here it is applied to profanity2's Ethereum kernel.

Because five of the six variants are not a simple `seed + delta` addition, each
hit now prints a `Variant:` tag:
```
  Time: … Private: 0x<delta> Variant: <0-5> Matching: 0x<address>
```
The real private key is `variant_v(seed_priv + delta) mod n`, where the variant
applies `λ` / `λ²` (endomorphism) and/or negation. Reconstruct and verify it with
the bundled helper (keys live in `~/.profanity2/`):
```bash
python3 ~/.profanity2/combine.py <logfile> --prefix ca5cade
```
It recomputes the address from the candidate key and refuses to emit anything
unless it matches the printed address, so a kernel or bookkeeping bug can only
cost a missed hit, never a wrong key. Variant meanings:

| Variant | Address form   | Private key           |
|---------|----------------|-----------------------|
| 0       | `(x,  y)`      | `k`                   |
| 1       | `(βx, y)`      | `λk mod n`            |
| 2       | `(β²x, y)`     | `λ²k mod n`           |
| 3       | `(x,  -y)`     | `n − k`               |
| 4       | `(βx, -y)`     | `n − λk`              |
| 5       | `(β²x, -y)`    | `n − λ²k`             |

(where `k = seed_priv + delta`). The endomorphism constants and every variant
transform are self-tested in `~/.profanity2/eth_crypto.py`.

## Searching a whole word list at once (`--matching-list`)

Instead of one prefix, hunt for **any** word in a file — each address is checked
against every pattern in a single pass, so the keccak (the bottleneck) is paid
once no matter how many words you list. The first hit for "any of my words"
arrives ~N× sooner than searching them one at a time.
```bash
./profanity2.x64 --matching-list ~/.profanity2/armenian.txt -z <pubkey>
```
The file holds one hex pattern per line (`#` comments and `0x` prefixes ok; up to
64 patterns of 16 nibbles). The reported `Score` is the length of the longest
word that fully matched, so hits climb as longer words are found. Reconstruct any
hit exactly as above with `combine.py --prefix <the word>`.

# Usage
```
usage: ./profanity2 [OPTIONS]

  Mandatory args:
    -z                      Seed public key to start, add it's private key
                            to the "profanity2" resulting private key.

  Basic modes:
    --benchmark             Run without any scoring, a benchmark.
    --zeros                 Score on zeros anywhere in hash.
    --letters               Score on letters anywhere in hash.
    --numbers               Score on numbers anywhere in hash.
    --mirror                Score on mirroring from center.
    --leading-doubles       Score on hashes leading with hexadecimal pairs
    -b, --zero-bytes        Score on hashes containing the most zero bytes

  Modes with arguments:
    --leading <single hex>  Score on hashes leading with given hex character.
    --matching <hex string> Score on hashes matching given hex string.

  Advanced modes:
    --contract              Instead of account address, score the contract
                            address created by the account's zeroth transaction.
    --leading-range         Scores on hashes leading with characters within
                            given range.
    --range                 Scores on hashes having characters within given
                            range anywhere.

  Range:
    -m, --min <0-15>        Set range minimum (inclusive), 0 is '0' 15 is 'f'.
    -M, --max <0-15>        Set range maximum (inclusive), 0 is '0' 15 is 'f'.

  Device control:
    -s, --skip <index>      Skip device given by index.
    -n, --no-cache          Don't load cached pre-compiled version of kernel.

  Tweaking:
    -w, --work <size>       Set OpenCL local work size. [default = 64]
    -W, --work-max <size>   Set OpenCL maximum work size. [default = -i * -I]
    -i, --inverse-size      Set size of modular inverses to calculate in one
                            work item. [default = 255]
    -I, --inverse-multiple  Set how many above work items will run in
                            parallell. [default = 16384]

  Examples:
    ./profanity2 --leading f -z HEX_PUBLIC_KEY_128_CHARS_LONG
    ./profanity2 --matching dead -z HEX_PUBLIC_KEY_128_CHARS_LONG
    ./profanity2 --matching badXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXbad -z HEX_PUBLIC_KEY_128_CHARS_LONG
    ./profanity2 --leading-range -m 0 -M 1 -z HEX_PUBLIC_KEY_128_CHARS_LONG
    ./profanity2 --leading-range -m 10 -M 12 -z HEX_PUBLIC_KEY_128_CHARS_LONG
    ./profanity2 --range -m 0 -M 1 -z HEX_PUBLIC_KEY_128_CHARS_LONG
    ./profanity2 --contract --leading 0 -z HEX_PUBLIC_KEY_128_CHARS_LONG
    ./profanity2 --contract --zero-bytes -z HEX_PUBLIC_KEY_128_CHARS_LONG

  About:
    profanity2 is a vanity address generator for Ethereum that utilizes
    computing power from GPUs using OpenCL.

  Forked "profanity2":
    Author: 1inch Network <info@1inch.io>
    Disclaimer:
      This project "profanity2" was forked from the original project and
      modified to guarantee "SAFETY BY DESIGN". This means source code of
      this project doesn't require any audits, but still guarantee safe usage.

  From original "profanity":
    Author: Johan Gustafsson <profanity@johgu.se>
    Beer donations: 0x000dead000ae1c8e8ac27103e4ff65f42a4e9203
    Disclaimer:
      Always verify that a private key generated by this program corresponds to
      the public key printed by importing it to a wallet of your choice. This
      program like any software might contain bugs and it does by design cut
      corners to improve overall performance.
```

### Benchmarks - Current version
|Model|Clock Speed|Memory Speed|Modified straps|Speed|Time to match eight characters
|:-:|:-:|:-:|:-:|:-:|:-:|
|GTX 1070 OC|1950|4450|NO|179.0 MH/s| ~24s
|GTX 1070|1750|4000|NO|163.0 MH/s| ~26s
|RX 480|1328|2000|YES|120.0 MH/s| ~36s
|RTX 4090|-|-|-|1096 MH/s| ~3s
|Apple Silicon M1<br/>(8-core GPU)|-|-|-|45.0 MH/s| ~97s
|Apple Silicon M1 Max<br/>(32-core GPU)|-|-|-|172.0 MH/s| ~25s
|Apple Silicon M3 Pro<br/>(18-core GPU)|-|-|-|97 MH/s| ~45s
|Apple Silicon M4 Max<br/>(40-core GPU)|-|-|-|350 MH/s| ~12s

