# Security Notes

This repository is prepared for public GitHub upload.

## Credentials removed

The original hardware sketch contained local Wi-Fi and ThingsBoard credentials. The public copy has been sanitized and uses placeholders instead.

Before publishing:

1. Regenerate any ThingsBoard token that has previously been shared publicly.
2. Never commit a real Wi-Fi password.
3. Never commit `.env`, `credentials.h`, or `secrets.h` files.
4. If a credential is accidentally committed, rotate it immediately; deleting the line from a later commit is not enough.
