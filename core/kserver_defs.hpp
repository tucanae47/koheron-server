/// Server definitions and static configurations
///
/// (c) Koheron

#ifndef __KSERVER_DEFS_HPP__
#define __KSERVER_DEFS_HPP__

#include <cstdint>

namespace kserver {

// Run KServer as a daemon process
#define KSERVER_IS_DAEMON 1

// ------------------------------------------
// Connections
// ------------------------------------------

/// Systemd notification socket
#define DFLT_NOTIFY_SOCKET "/run/systemd/notify"

/// Default number of parallel sessions per server
#define DFLT_WORKER_CONNECTIONS 10

/// Default port number
#define TCP_DFLT_PORT 36000

/// Default webSocket port
#define WEBSOCKET_DFLT_PORT 8080

/// Pending connections queue size
#define KSERVER_BACKLOG 10

/// Unix socket path
#define DFLT_UNIX_SOCK_PATH "/var/run/kserver.sock"

/// Disable Nagle algorithm for TCP connections
#define KSERVER_HAS_TCP_NODELAY 1

#define KSERVER_HAS_SYSTEMD 1

// ------------------------------------------
// Buffer sizes
// ------------------------------------------

/// Number of samples
#define KSERVER_SIG_LEN 16384

/// Command payload buffer length
constexpr int64_t CMD_PAYLOAD_BUFFER_LEN = 16384 * 8;

/// Read string length
#define KSERVER_READ_STR_LEN 16384

/// Send string length
#define KSERVER_SEND_STR_LEN 16384

/// Receive data buffer length
#define KSERVER_RECV_DATA_BUFF_LEN 16384 * 2 * 4

/// Number of char for the device identification
#define N_CHAR_DEV 16

/// Number of char for the operation identification
#define N_CHAR_OP 16

/// Websocket receive buffer size
#define WEBSOCK_READ_STR_LEN KSERVER_RECV_DATA_BUFF_LEN

/// Websocket send buffer size (bytes)
#define WEBSOCK_SEND_BUF_LEN 16384 * 2 * 4

/// Maximum length of the Unix socket file path
///
/// Note:
/// 108 is the maximum length on Linux. See:
/// http://man7.org/linux/man-pages/man7/unix.7.html
#define UNIX_SOCKET_PATH_LEN 108

// ------------------------------------------
// Debugging
// ------------------------------------------

#ifndef DEBUG_KSERVER
# define NDEBUG
#endif

// ------------------------------------------
// Libraries
// ------------------------------------------

/// Use Boost library
#define USE_BOOST 0

// ------------------------------------------
// Misc
// ------------------------------------------

typedef int SessID;

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

// ------------------------------------------
// Checks
// ------------------------------------------

} // namespace kserver

#endif // __KSERVER_DEFS_HPP__


