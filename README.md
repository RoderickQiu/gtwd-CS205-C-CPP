# GTWD

> **G**lobal **T**ranscoder for **W**av/FLAC **D**ata

Actually originated in the famous saying:

> GuTao[[1]](https://github.com/GuTaoZi) tastes good, no need for salt.

## CLI Usage

### Arguments

- For all modes:
    - `-m` or `--mode`:
        - `w2f` for wav to flac
        - `f2w` for flac to wav
        - `f2p` for flac to pcm
        - `p2f` for pcm to flac
        - `p2w` for pcm to wav
        - `w2p` for wav to pcm
        - `fm` for flac meta data reading
        - `fe` for flac meta data editing
    - `-i` or `--input`: input file path.
    - `-o` or `--output`: output file path.
- For `pcm` files (`p2w`, `p2f`)
    - `-c` or `--config`: config ini file path for `pcm` files.
- For `fe` mode (Flac meta data editing):
    - `-mv` or `--modify-vendor`: modify vendor
        - argument is the vendor string to modify
    - `-mc` or `--modify-comment`: modify comment
        - need two arguments, the first one is the comment content, the second one is the comment index to delete
    - `-ac` or `--append-comment`: append comment
        - argument is the comment content to append
    - `-rc` or `--remove-comment`: remove comment
        - argument is the comment index to remove

### Typical Usage

```bash
gtwd -m fe -i "/Users/r/Downloads/flac-test-files/subset/15 - only verbatim subframes.flac" -o "/Users/r/Documents/gtwd/ProvidedDocuments/out.flac" -mv "In GuTao we trust" -ac "Gutao tastes good, no need for salt"

gtwd -m fm -i "/Users/r/Documents/gtwd/ProvidedDocuments/out.flac"
```