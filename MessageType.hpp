#pragma once

/**
 * Denotes the type of MPI messages.
 */

enum class MessageType {
    Init = 0,
    Time = 1,
    Color = 2,
    Done = 3,
    Exit = 4
};
