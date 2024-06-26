# L2CAP on Bluetooth LE data loss

This demonstrates L2CAP on Bluetooth LE data loss when the receiving end is held up.

Two machines with Bluetooth LE capable host adapters are required.

## Running

Bluetooth headers/libraries required.
Run `make` to build.

On machine A run `l2cap-server`.

On machine B run `l2cap-client <addr>` where `<addr>` is the Bluetooth address of machine A.

## Output

### Machine A (sender)

    Ready
    accepted connection from A8:7E:EA:35:30:F8
    sent 1 bytes, total 1 bytes
    sent 10 bytes, total 11 bytes
    sent 100 bytes, total 111 bytes
    sent 600 bytes, total 711 bytes
    sent 400 bytes, total 1111 bytes
    sent 600 bytes, total 1711 bytes
    sent 600 bytes, total 2311 bytes
    sent 600 bytes, total 2911 bytes
    sent 600 bytes, total 3511 bytes
    sent 600 bytes, total 4111 bytes
    sent 600 bytes, total 4711 bytes
    sent 600 bytes, total 5311 bytes
    sent 600 bytes, total 5911 bytes
    [...]
    sent 600 bytes, total 110111 bytes
    sent 600 bytes, total 110711 bytes
    sent 400 bytes, total 111111 bytes
    sleeping
    sent 1 bytes, total 111112 bytes
    Sent total of 111112 bytes

### Machine B (receiver)

    Received 11 bytes, total 11 bytes
    Received 100 bytes, total 111 bytes
    Received 600 bytes, total 711 bytes
    Sleeping purposefully
    Received 600 bytes, total 1311 bytes
    Received 600 bytes, total 1911 bytes
    Received 600 bytes, total 2511 bytes
    Received 600 bytes, total 3111 bytes
    [...]
    Received 600 bytes, total 98111 bytes
    Received 600 bytes, total 98711 bytes
    Received 600 bytes, total 99311 bytes
    Received 400 bytes, total 99711 bytes
    Received 1 bytes, total 99712 bytes
    read: Connection reset by peer


No errors have were reported by the OS.
As seen machine A has sent 111112 bytes, but machine B only received 99712 bytes.

## Expected result

Since L2CAP streams are supposed to be reliable, all data should be received and the sender should block when the buffer at the receiver is full.
At the very least the connection should fail, i.e. an OS error should be reported at the sender.

