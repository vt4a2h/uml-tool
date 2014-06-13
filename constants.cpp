#pragma once

namespace {
    static constexpr char* GLOBAL_SCOPE_ID = (char*)"_global_scope";
    static constexpr char* STUB_ID = (char*)"_stub_type";
    static constexpr char* VOID_ID = (char*)"_void";

    static constexpr char* DEFAULT_NAME = (char*)"__noname";
    static constexpr char* DEFAULT_DESCRIPTION = (char*)"_nothing";

    static constexpr char* DEFAULT_DATABASE_NAME = (char*)"_database";
    static constexpr char* DEFAULT_DATABASE_PATH = (char*)"";
    static constexpr char* DEFAULT_DATABASE_EXTENSION = (char*)"qutdb";

    static constexpr char* QTYPE_HASH_ID      = (char*)"_q_hash";
    static constexpr char* QTYPE_MULTIHASH_ID = (char*)"_q_multihash";
    static constexpr char* QTYPE_LIST_ID      = (char*)"_q_list";
    static constexpr char* QTYPE_SET_ID       = (char*)"_q_set";
    static constexpr char* QTYPE_MULTISET_ID  = (char*)"_q_multiset";
}
