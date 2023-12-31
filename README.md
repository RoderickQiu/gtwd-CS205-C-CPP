# GTWD

> **G**lobal **T**ranscoder for **W**AV/FLAC/AIFF **D**ata

Actually originated in the famous saying:

> GuTao[[1]](https://github.com/GuTaoZi) tastes good, no need for salt.

## CLI Usage

### Arguments

- For all modes:
    - `-m` or `--mode`:
        - `w2f` for wav to flac
        - `f2w` for flac to wav
        - `f2r` for flac to raw _(pending)_
        - `r2f` for raw to flac _(pending)_
        - `r2w` for raw to wav _(pending)_
        - `w2r` for wav to raw _(pending)_
        - `w2a` for wav to aiff
        - `a2w` for aiff to wav
        - `a2f` for aiff to flac _(pending)_
        - `f2a` for flac to aiff _(pending)_
        - `a2r` for aiff to raw _(pending)_
        - `r2a` for raw to aiff _(pending)_
        - `fm` for flac meta data reading
        - `fe` for flac meta data editing
    - `-i` or `--input`: input file path
    - `-o` or `--output`: output file path
- For `raw` files (`r2w`, `r2f`)
    - `-c` or `--config`: config ini file path for `raw` files
- For `fe` mode (Flac meta data editing):
    - `-mv` or `--modify-vendor`: modify vendor
        - argument is the vendor string to modify
    - `-mc` or `--modify-comment`: modify comment
        - need two arguments
            - the first one is the comment content
            - the second one is the comment index to delete
    - `-ac` or `--append-comment`: append comment
        - argument is the comment content to append
    - `-rc` or `--remove-comment`: remove comment
        - argument is the comment index to remove

### Typical Usage

```bash
gtwd -m fe -i "/Users/r/Documents/gtwd/ProvidedDocuments/ovs.flac" -o "/Users/r/Documents/gtwd/ProvidedDocuments/output.flac" -mv "In GuTao we trust" -ac "TITLE=Trinity of GuTao: GuTao the Father, GuTaoZi and GuTao the Holy Spirit"

gtwd -m fm -i "/Users/r/Documents/gtwd/ProvidedDocuments/output.flac"
```