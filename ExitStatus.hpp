// Copyright (C) 2012-2015, Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef EXITSTATUS_HPP
#define EXITSTATUS_HPP

struct ExitStatus {

    enum Code {
        OK = 0,
        RGF_ERROR,
        SET_ERROR,
        XY_ERROR,
        TRJ_ERROR,
        BUG,
        LOGIC_ERROR,
        RUNTIME_ERROR,
        STD_EXCEPTION,
        ARG_ERROR,
        UNKNOWN_ERROR
    };

};

#endif // EXITSTATUS_HPP
