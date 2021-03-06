/*
**      ARTSAT Morikawa Application
**
**      Original Copyright (C) 2013 - 2014 Ron Hashimoto.
**                                          http://h2so5.net/
**                                          mail@h2so5.net
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      All rights reserved.
**
**      Version     Arduino
**      Website     http://artsat.jp/
**      E-mail      info@artsat.jp
**
**      This source code is for Arduino IDE.
**      Arduino 1.0.5
**
**      InvaderVM.ino
**
**      ------------------------------------------------------------------------
**
**      GNU GENERAL PUBLIC LICENSE (GPLv3)
**
**      This program is free software: you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation, either version 3 of the License,
**      or (at your option) any later version.
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.ß
**      See the GNU General Public License for more details.
**      You should have received a copy of the GNU General Public License
**      along with this program. If not, see <http://www.gnu.org/licenses/>.
**
**      このプログラムはフリーソフトウェアです。あなたはこれをフリーソフトウェア財団によって発行された
**      GNU 一般公衆利用許諾書（バージョン 3 か、それ以降のバージョンのうちどれか）が定める条件の下で
**      再頒布または改変することができます。このプログラムは有用であることを願って頒布されますが、
**      *全くの無保証* です。商業可能性の保証や特定目的への適合性は、言外に示されたものも含め全く存在しません。
**      詳しくは GNU 一般公衆利用許諾書をご覧ください。
**      あなたはこのプログラムと共に GNU 一般公衆利用許諾書のコピーを一部受け取っているはずです。
**      もし受け取っていなければ <http://www.gnu.org/licenses/> をご覧ください。
*/

static const int INVADER_VM_REGISTER_SIZE = 16;
static const int INVADER_VM_HEAP_LENGTH   = 128;
static const int INVADER_VM_CODE_LENGTH   = 128;

struct VMState {
    char code[INVADER_VM_CODE_LENGTH + 1];
    char heap[INVADER_VM_HEAP_LENGTH + sizeof(long)];
    long reg[INVADER_VM_REGISTER_SIZE];
    long cur;
    int error;
};

VMState vm_state;

void InvaderVM_callFunction(VMState *state);
void InvaderVM_loadProgram(VMState *state);
void InvaderVM_run(VMState *state);

enum {
    REG_ERRN,
    REG_FUNC,
    REG_RETV,
    REG_HCUR,
    REG_HEAP,
    REG_ARG0,
    REG_ARG1,
    REG_ARG2,
    REG_ARG3,
    REG_ARG4,
    REG_ARG5,
    REG_ARG6,
    REG_ARG7,
    REG_ARG8,
    REG_ARG9,
    REG_ARGA
};

enum VMError {
    ERR_NONE,
    ERR_LOAD_FAILURE,
    ERR_MELT_FAILURE,
    ERR_INVALID_INSTRUCTION,
    ERR_INVALID_JUMP,
    ERR_INVALID_REGISTER,
    ERR_INVALID_HEAP,
    ERR_UNKNOWN_FUNCTION,
    ERR_ZERO_DIVISION
};

enum InvaderVMCode {
    VM_END,       /* End of Code              []                    */
    VM_NOP,       /* No Operation             []                    */
    VM_SET,       /* Set long                 [char, long]          */
    VM_SETC,      /* Set char                 [char, char]          */
    VM_SETI,      /* Set int                  [char, int]           */
    VM_CLR,       /* Set Zero                 [char]                */
    VM_MOV,       /* Copy                     [char, char]          */
    VM_XCHG,      /* Exchange                 [char, char]          */
    VM_CALL,      /* Call Function                                  */
    
    VM_JMP,       /* Jump                     [char]                */
    VM_JMPIF,     /* Jump if true             [char, char]          */
    VM_JMPNOT,    /* Jump if false            [char, char]          */
    
    VM_INC,       /* Increment                [char]                */
    VM_DEC,       /* Decrement                [char]                */
    VM_NEG,       /* Negate                   [char]                */
    VM_ADD,       /* Add                      [char, char]          */
    VM_SUB,       /* Subtract                 [char, char]          */
    VM_MUL,       /* Multiply                 [char, char]          */
    VM_DIV,       /* Divide                   [char, char, char]    */
    
    VM_AND,       /* Logical AND              [char, char]          */
    VM_OR,        /* Logical OR               [char, char]          */
    VM_XOR,       /* Logical XOR              [char, char]          */
    VM_NOT,       /* Logical NOT              [char]                */
    VM_SHR,       /* Shift Right              [char, char]          */
    VM_SHL,       /* Shift Left               [char, char]          */
    
    VM_EQ,        /* Equal                    [char, char, char]    */
    VM_LT,        /* Lower Than               [char, char, char]    */
    VM_LE,        /* Equal And Lower Than     [char, char, char]    */
    VM_GT,        /* Greater Than             [char, char, char]    */
    VM_GE,        /* Equal And Greater Than   [char, char, char]    */
    
    VM_WAIT,      /* Delay n miliseconds      [char]                */
    VM_TXT,       /* Load Text                [long, char...]       */
    VM_EXC,       /* Load Heap As Program     [long]                */
    
    VM_COMPRESSED /* Compressed Flag                                */
};

enum InvaderVMFunc {
    VMFunc_getMemorySpec = 0x01,
    VMFunc_getMemoryInfo,
    VMFunc_getMemoryLog,
    VMFunc_saveMemoryLog,
    
    VMFunc_getSelfTestLog,
    VMFunc_eraseSelfTestLog,
    
    VMFunc_getSizeEEPROM,
    VMFunc_getSizeSharedMemory,
    VMFunc_getSizeFRAM,
    VMFunc_getSizeFlashROM,
    VMFunc_getPageSizeEEPROM,
    VMFunc_getPageSizeSharedMemory,
    VMFunc_getPageSizeFRAM,
    VMFunc_getPageSizeFlashROM,
    VMFunc_getSectorSizeEEPROM,
    VMFunc_getSectorSizeSharedMemory,
    VMFunc_getSectorSizeFRAM,
    VMFunc_getSectorSizeFlashROM,
    
    VMFunc_getCameraFormat,
    
    VMFunc_getBootTime,
    VMFunc_getBootCount,
    VMFunc_getBootMode,
    
    VMFunc_getParamNote,
    VMFunc_getParamMorse,
    VMFunc_getParamDigitalker,
    VMFunc_getParamCamera,
    
    VMFunc_getTelemetryTime,
    VMFunc_getTelemetryVoltage,
    VMFunc_getTelemetryCurrent,
    VMFunc_getTelemetryTemperature,
    VMFunc_getTelemetryGyro,
    VMFunc_getTelemetryMagnet,
    
    VMFunc_setFlashROMEraseMode,
    VMFunc_getFlashROMEraseMode,
    
    VMFunc_setText,
    VMFunc_getText,
    
    VMFunc_setLED,
    VMFunc_getLED,
    
    VMFunc_setNoteBPM,
    VMFunc_getNoteBPM,
    VMFunc_setMorseWPM,
    VMFunc_getMorseWPM,
    VMFunc_setSpeakAsyncMode,
    VMFunc_getSpeakAsyncMode,
    
    VMFunc_isValid,
    VMFunc_isValidSharedMemory,
    VMFunc_isValidFRAM,
    VMFunc_isValidFlashROM,
    VMFunc_isValidLED,
    VMFunc_isValidTone,
    VMFunc_isValidDigitalker,
    VMFunc_isValidCamera,
    
    VMFunc_isBusyDigitalker,
    VMFunc_hasTelemetryUpdate,
    VMFunc_hasAbnormalShutdown,
    
    VMFunc_shutdown,
    
    VMFunc_writeEEPROM,
    VMFunc_readEEPROM,
    VMFunc_formatEEPROM,
    VMFunc_writeSharedMemory,
    VMFunc_readSharedMemory,
    VMFunc_formatSharedMemory,
    VMFunc_writeFRAM,
    VMFunc_readFRAM,
    VMFunc_formatFRAM,
    VMFunc_writeFlashROM,
    VMFunc_readFlashROM,
    VMFunc_formatFlashROM,
    
    VMFunc_playFrequency,
    VMFunc_playFrequency2,
    VMFunc_playNote,
    VMFunc_playNote2,
    VMFunc_playMorse,
    
    VMFunc_speakPhrase,
    VMFunc_waitPhrase,
    VMFunc_stopPhrase,
    
    VMFunc_snapshotCamera,
    
    VMFunc_freezeFastLZ,
    VMFunc_meltFastLZ,
    
    VMFunc_enableAudioBus,
    VMFunc_disableAudioBus
};

static bool isValidCode(long pos) {
    return (0 <= pos && pos < INVADER_VM_CODE_LENGTH);
}

static bool isValidRegister(char pos) {
    return (0 <= pos && pos < INVADER_VM_REGISTER_SIZE);
}

static bool isValidHeap(long pos) {
    return (0 <= pos && pos < INVADER_VM_HEAP_LENGTH);
}

void VM_ERROR(int errno, void *state) {
    static char err[256] = { 0 };
    char *msg = NULL;
    switch (errno) {
        case ERR_LOAD_FAILURE:
            msg = "fail to load code";
            break;
        case ERR_MELT_FAILURE:
            msg = "fail to decompress code";
            break;
        case ERR_INVALID_INSTRUCTION:
            msg = "invalid instruction";
            break;
        case ERR_INVALID_JUMP:
            msg = "out of code bounds";
            break;
        case ERR_INVALID_REGISTER:
            msg = "out of register bounds";
            break;
        case ERR_INVALID_HEAP:
            msg = "out of heap bounds";
            break;
        case ERR_UNKNOWN_FUNCTION:
            msg = "unknown function";
            break;
        case ERR_ZERO_DIVISION:
            msg = "division by zero";
            break;
    }
    snprintf(err, sizeof(err), "#[%02x] ", ((VMState*)state)->cur);
    strncat(err, msg, sizeof(err));
#ifdef TARGET_BUILD_DEBUG_VM
    Serial.println(err);
#endif
    Morikawa.setText(TEXT_DEBUG, err);
    ((VMState*)state)->error = errno;
}

void InvaderVM_setup(void)
{
    InvaderVM_loadProgram(&vm_state);
    return;
}

void InvaderVM_loop(void)
{
#ifdef TARGET_BUILD_DEBUG_VM
    InvaderVM_runTests();
#else
    InvaderVM_run(&vm_state);
#endif
    return;
}

void InvaderVM_loadProgram(VMState *state)
{
    int length = 0;
    memset(&state->code, 0, INVADER_VM_CODE_LENGTH);
    Morikawa.getText(TEXT_Z, state->code, INVADER_VM_CODE_LENGTH + 1, &length);
    if (length == 0) {
        VM_ERROR(ERR_LOAD_FAILURE, state);
        return;
    }
    
    if (static_cast<InvaderVMCode>(state->code[0]) == VM_COMPRESSED) {
        unsigned int result = 0;
        unsigned long output = 0;
        length = length - 2;
        Morikawa.writeFRAM(0, state->code + 1, length, &result);
        Morikawa.meltFastLZ(STORAGE_FRAM, 0, length, STORAGE_FRAM, length, INVADER_VM_CODE_LENGTH, &output);
        Morikawa.readFRAM(length, state->code, output, &result);
        if (result == 0) {
            VM_ERROR(ERR_MELT_FAILURE, state);
        }
    }
}

void InvaderVM_run(VMState *state)
{
    memset(state->reg, 0, INVADER_VM_REGISTER_SIZE * sizeof(long));
    state->cur = 0;
    state->error = ERR_NONE;
    
    while (state->cur < INVADER_VM_CODE_LENGTH && state->error == ERR_NONE) {
        InvaderVMCode c = static_cast<InvaderVMCode>(state->code[state->cur]);
        char *arg = state->code + state->cur + sizeof(char);
        
        if (c == VM_END) {
            break;
        }
        
        long hcur = state->reg[REG_HCUR];
        if (isValidHeap(hcur)) {
            memcpy(&state->reg[REG_HEAP], &state->heap[hcur], sizeof(long));
        }
        else {
            VM_ERROR(ERR_INVALID_HEAP, state);
        }
        
        switch (c) {
            case VM_NOP:
                break;
                
            case VM_SET:
            {
                char pos = arg[0];
                long val = *reinterpret_cast<long *>(arg + sizeof(char));
                if (isValidRegister(pos)) {
                    state->reg[pos] = val;
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(long);
            }
                break;
                
            case VM_SETC:
            {
                char pos = arg[0];
                char val = arg[sizeof(char)];
                if (isValidRegister(pos)) {
                    state->reg[pos] = 0;
                    *reinterpret_cast<char *>(&state->reg[pos]) = val;
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_SETI:
            {
                char pos = arg[0];
                int val = *reinterpret_cast<int *>(arg + sizeof(char));
                if (isValidRegister(pos)) {
                    state->reg[pos] = 0;
                    *reinterpret_cast<int *>(&state->reg[pos]) = val;
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(int);
            }
                break;
                
            case VM_CLR:
            {
                char pos = arg[0];
                if (isValidRegister(pos)) {
                    state->reg[pos] = 0;
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char);
            }
                break;
                
            case VM_MOV:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] = state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_XCHG:
            {
                char a = arg[0];
                char b = arg[sizeof(char)];
                if (isValidRegister(a) && isValidRegister(b)) {
                    long tmp = state->reg[a];
                    state->reg[a] = state->reg[b];
                    state->reg[b] = tmp;
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_CALL:
                InvaderVM_callFunction(state);
                break;
                
            case VM_JMP:
            {
                char offset = arg[0];
                if (isValidCode(state->cur + offset)) {
                    state->cur += offset;
                }
                else {
                    VM_ERROR(ERR_INVALID_JUMP, state);
                }
                state->cur += sizeof(char);
            }
                break;
                
            case VM_JMPIF:
            {
                char pos = arg[0];
                char offset = arg[sizeof(char)];
                if (isValidRegister(pos)) {
                    if (state->reg[pos]) {
                        if (isValidCode(state->cur + offset)) {
                            state->cur += offset;
                        }
                        else {
                            VM_ERROR(ERR_INVALID_JUMP, state);
                        }
                    }
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_JMPNOT:
            {
                char pos = arg[0];
                char offset = arg[sizeof(char)];
                if (isValidRegister(pos)) {
                    if (!state->reg[pos]) {
                        if (isValidCode(state->cur + offset)) {
                            state->cur += offset;
                        }
                        else {
                            VM_ERROR(ERR_INVALID_JUMP, state);
                        }
                    }
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_INC:
            {
                char pos = arg[0];
                if (isValidRegister(pos)) {
                    state->reg[pos]++;
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char);
            }
                break;
                
            case VM_DEC:
            {
                char pos = arg[0];
                if (isValidRegister(pos)) {
                    state->reg[pos]--;
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char);
            }
                break;
                
            case VM_NEG:
            {
                char pos = arg[0];
                if (isValidRegister(pos)) {
                    state->reg[pos] *= -1;
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char);
            }
                break;
                
            case VM_ADD:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] += state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_SUB:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] -= state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_MUL:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] *= state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_DIV:
            {
                char qdst = arg[0];
                char rdst = arg[sizeof(char)];
                char src  = arg[sizeof(char) + sizeof(char)];
                if (isValidRegister(qdst) && isValidRegister(rdst) && isValidRegister(src)) {
                    if (src == 0) {
                        VM_ERROR(ERR_ZERO_DIVISION, state);
                    }
                    else {
                        long s = state->reg[qdst];
                        state->reg[qdst] = s / state->reg[src];
                        state->reg[rdst] = s % state->reg[src];
                    }
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_AND:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] &= state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_OR:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] |= state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_XOR:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] ^= state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_NOT:
            {
                char pos = arg[0];
                if (isValidRegister(pos)) {
                    state->reg[pos] = ~state->reg[pos];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char);
            }
                break;
                
            case VM_SHR:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] >>= state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_SHL:
            {
                char dst = arg[0];
                char src = arg[sizeof(char)];
                if (isValidRegister(dst) && isValidRegister(src)) {
                    state->reg[dst] <<= state->reg[src];
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_EQ:
            {
                char res = arg[0];
                char lhs = arg[sizeof(char)];
                char rhs = arg[sizeof(char) + sizeof(char)];
                if (isValidRegister(lhs) && isValidRegister(rhs) && isValidRegister(res)) {
                    state->reg[res] = (state->reg[lhs] == state->reg[rhs]);
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_LT:
            {
                char res = arg[0];
                char lhs = arg[sizeof(char)];
                char rhs = arg[sizeof(char) + sizeof(char)];
                if (isValidRegister(lhs) && isValidRegister(rhs) && isValidRegister(res)) {
                    state->reg[res] = (state->reg[lhs] < state->reg[rhs]);
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_LE:
            {
                char res = arg[0];
                char lhs = arg[sizeof(char)];
                char rhs = arg[sizeof(char) + sizeof(char)];
                if (isValidRegister(lhs) && isValidRegister(rhs) && isValidRegister(res)) {
                    state->reg[res] = (state->reg[lhs] <= state->reg[rhs]);
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_GT:
            {
                char res = arg[0];
                char lhs = arg[sizeof(char)];
                char rhs = arg[sizeof(char) + sizeof(char)];
                if (isValidRegister(lhs) && isValidRegister(rhs) && isValidRegister(res)) {
                    state->reg[res] = (state->reg[lhs] > state->reg[rhs]);
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_GE:
            {
                char res = arg[0];
                char lhs = arg[sizeof(char)];
                char rhs = arg[sizeof(char) + sizeof(char)];
                if (isValidRegister(lhs) && isValidRegister(rhs) && isValidRegister(res)) {
                    state->reg[res] = (state->reg[lhs] >= state->reg[rhs]);
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char) + sizeof(char) + sizeof(char);
            }
                break;
                
            case VM_WAIT:
            {
                char pos = arg[0];
                if (isValidRegister(pos)) {
                    delay(state->reg[pos]);
                }
                else {
                    VM_ERROR(ERR_INVALID_REGISTER, state);
                }
                state->cur += sizeof(char);
            }
                break;
                
            case VM_TXT:
            {
                long length = *reinterpret_cast<long *>(arg);
                char *data  = arg + sizeof(long);
                if (isValidHeap(hcur) && isValidHeap(hcur + length - 1)) {
                    memcpy(&state->heap[hcur], data, length);
                    state->reg[REG_HEAP] = *reinterpret_cast<long *>(&data[hcur]);
                }
                else {
                    VM_ERROR(ERR_INVALID_HEAP, state);
                }
                state->cur += sizeof(long) + length;
            }
                break;
                
            case VM_EXC:
            {
                long length = *reinterpret_cast<long *>(arg);
                if (isValidHeap(hcur) && isValidHeap(hcur + length - 1)) {
                    if (isValidCode(length - 1)) {
                        memcpy(&state->code[0], &state->heap[hcur], length);
                        state->cur = sizeof(long) + sizeof(char);
                        state->cur *= -1;
                    }
                    else {
                        VM_ERROR(ERR_INVALID_INSTRUCTION, state);
                    }
                }
                else {
                    VM_ERROR(ERR_INVALID_HEAP, state);
                }
                state->cur += sizeof(long);
            }
                break;
                
            default:
                VM_ERROR(ERR_INVALID_INSTRUCTION, state);
        }
        
        if (!isValidHeap(hcur)) {
            VM_ERROR(ERR_INVALID_HEAP, state);
        }
        else if (hcur == state->reg[REG_HCUR]) {
            memcpy(&state->heap[hcur], &state->reg[REG_HEAP], sizeof(long));
        }
        
        state->cur += sizeof(char);
    }
    
    if (state->error != ERR_NONE) {
        Morikawa.shutdown();
    }
}

void InvaderVM_callFunction(VMState *state)
{
    long hcur = state->reg[REG_HCUR];
    char *heap = state->heap + hcur;
    InvaderVMFunc func = static_cast<InvaderVMFunc>(state->reg[REG_FUNC]);
    switch (func) {
            
        case VMFunc_getMemorySpec:
            if (isValidHeap(hcur + sizeof(MemorySpec))) {
                state->reg[REG_ERRN] = Morikawa.getMemorySpec(reinterpret_cast<MemorySpec*>(heap));
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_getMemoryInfo:
            if (isValidHeap(hcur + sizeof(MemoryInfo))) {
                state->reg[REG_ERRN] = Morikawa.getMemoryInfo(reinterpret_cast<MemoryInfo*>(heap));
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_getMemoryLog:
            if (isValidHeap(hcur + sizeof(MemoryInfo))) {
                state->reg[REG_ERRN] = Morikawa.getMemoryLog(reinterpret_cast<MemoryInfo*>(heap));
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_saveMemoryLog:
            Morikawa.saveMemoryLog();
            break;
            
        case VMFunc_getSelfTestLog:
            if (isValidHeap(hcur + sizeof(SelfTestLog))) {
                state->reg[REG_ERRN] = Morikawa.getSelfTestLog(reinterpret_cast<SelfTestLog*>(heap));
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_eraseSelfTestLog:
            Morikawa.eraseSelfTestLog();
            break;
            
        case VMFunc_getSizeEEPROM:
            state->reg[REG_RETV] = Morikawa.getSizeEEPROM();
            break;
            
        case VMFunc_getSizeSharedMemory:
            state->reg[REG_RETV] = Morikawa.getSizeSharedMemory();
            break;
            
        case VMFunc_getSizeFRAM:
            state->reg[REG_RETV] = Morikawa.getSizeFRAM();
            break;
            
        case VMFunc_getSizeFlashROM:
            state->reg[REG_RETV] = Morikawa.getSizeFlashROM();
            break;
            
        case VMFunc_getPageSizeEEPROM:
            state->reg[REG_RETV] = Morikawa.getPageSizeEEPROM();
            break;
            
        case VMFunc_getPageSizeSharedMemory:
            state->reg[REG_RETV] = Morikawa.getPageSizeSharedMemory();
            break;
            
        case VMFunc_getPageSizeFRAM:
            state->reg[REG_RETV] = Morikawa.getPageSizeFRAM();
            break;
            
        case VMFunc_getPageSizeFlashROM:
            state->reg[REG_RETV] = Morikawa.getPageSizeFlashROM();
            break;
            
        case VMFunc_getSectorSizeEEPROM:
            state->reg[REG_RETV] = Morikawa.getSectorSizeEEPROM();
            break;
            
        case VMFunc_getSectorSizeSharedMemory:
            state->reg[REG_RETV] = Morikawa.getSectorSizeSharedMemory();
            break;
            
        case VMFunc_getSectorSizeFRAM:
            state->reg[REG_RETV] = Morikawa.getSectorSizeFRAM();
            break;
            
        case VMFunc_getSectorSizeFlashROM:
            state->reg[REG_RETV] = Morikawa.getSectorSizeFlashROM();
            break;
            
        case VMFunc_getCameraFormat:
            if (isValidHeap(hcur + sizeof(CameraFormat))) {
                CameraFormat *format = reinterpret_cast<CameraFormat *>(heap);
                state->reg[REG_ERRN] = Morikawa.getCameraFormat(state->reg[REG_ARG0], format);
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_getBootTime:
            state->reg[REG_RETV] = Morikawa.getBootTime();
            break;
            
        case VMFunc_getBootCount:
            state->reg[REG_RETV] = Morikawa.getBootCount();
            break;
            
        case VMFunc_getBootMode:
            state->reg[REG_RETV] = Morikawa.getBootMode();
            break;
            
        case VMFunc_getParamNote:
            if (isValidHeap(hcur + sizeof(NoteParam))) {
                state->reg[REG_ERRN] = Morikawa.getParamNote(reinterpret_cast<NoteParam *>(heap));
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_getParamMorse:
            if (isValidHeap(hcur + sizeof(MorseParam))) {
                state->reg[REG_ERRN] = Morikawa.getParamMorse(reinterpret_cast<MorseParam *>(heap));
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_getParamDigitalker:
            if (isValidHeap(hcur + sizeof(DigitalkerParam))) {
                state->reg[REG_ERRN] = Morikawa.getParamDigitalker(reinterpret_cast<DigitalkerParam *>(heap));
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_getParamCamera:
            if (isValidHeap(hcur + sizeof(CameraParam))) {
                state->reg[REG_ERRN] = Morikawa.getParamCamera(reinterpret_cast<CameraParam *>(heap));
            }
            else {
                VM_ERROR(ERR_INVALID_HEAP, state);
            }
            break;
            
        case VMFunc_getTelemetryTime:
        {
            unsigned long result = 0;
            state->reg[REG_ERRN] = Morikawa.getTelemetryTime(state->reg[REG_ARG0], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_getTelemetryVoltage:
        {
            unsigned char result = 0;
            state->reg[REG_ERRN] = Morikawa.getTelemetryVoltage(state->reg[REG_ARG0], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_getTelemetryCurrent:
        {
            unsigned char result = 0;
            state->reg[REG_ERRN] = Morikawa.getTelemetryCurrent(state->reg[REG_ARG0], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_getTelemetryTemperature:
        {
            unsigned char result = 0;
            state->reg[REG_ERRN] = Morikawa.getTelemetryTemperature(state->reg[REG_ARG0], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_getTelemetryGyro:
        {
            unsigned char result = 0;
            state->reg[REG_ERRN] = Morikawa.getTelemetryGyro(state->reg[REG_ARG0], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_getTelemetryMagnet:
        {
            unsigned char result = 0;
            state->reg[REG_ERRN] = Morikawa.getTelemetryMagnet(state->reg[REG_ARG0], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_setFlashROMEraseMode:
            state->reg[REG_ERRN] = Morikawa.setFlashROMEraseMode(state->reg[REG_ARG0]);
            break;
            
        case VMFunc_getFlashROMEraseMode:
            state->reg[REG_RETV] = Morikawa.getFlashROMEraseMode();
            break;
            
        case VMFunc_setText:
            state->reg[REG_ERRN] = Morikawa.setText(state->reg[REG_ARG0], heap, state->reg[REG_ARG1]);
            break;
            
        case VMFunc_getText:
        {
            int result = 0;
            state->reg[REG_ERRN] = Morikawa.getText(state->reg[REG_ARG0], heap, state->reg[REG_ARG1], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_setLED:
            state->reg[REG_ERRN] = Morikawa.setLED(state->reg[REG_ARG0], state->reg[REG_ARG1]);
            break;
            
        case VMFunc_getLED:
            state->reg[REG_RETV] = Morikawa.getLED(state->reg[REG_ARG0]);
            break;
            
        case VMFunc_setNoteBPM:
            state->reg[REG_ERRN] = Morikawa.setNoteBPM(state->reg[REG_ARG0]);
            break;
            
        case VMFunc_getNoteBPM:
            state->reg[REG_RETV] = Morikawa.getNoteBPM();
            break;
            
        case VMFunc_setMorseWPM:
            state->reg[REG_ERRN] = Morikawa.setMorseWPM(state->reg[REG_ARG0]);
            break;
            
        case VMFunc_getMorseWPM:
            state->reg[REG_RETV] = Morikawa.getMorseWPM();
            break;
            
        case VMFunc_setSpeakAsyncMode:
            state->reg[REG_ERRN] = Morikawa.setSpeakAsyncMode(state->reg[REG_ARG0]);
            break;
            
        case VMFunc_getSpeakAsyncMode:
            state->reg[REG_RETV] = Morikawa.getSpeakAsyncMode();
            break;
            
        case VMFunc_isValid:
            state->reg[REG_RETV] = Morikawa.isValid();
            break;
            
        case VMFunc_isValidSharedMemory:
            state->reg[REG_RETV] = Morikawa.isValidSharedMemory();
            break;
            
        case VMFunc_isValidFRAM:
            state->reg[REG_RETV] = Morikawa.isValidFRAM();
            break;
            
        case VMFunc_isValidFlashROM:
            state->reg[REG_RETV] = Morikawa.isValidFlashROM();
            break;
            
        case VMFunc_isValidLED:
            state->reg[REG_RETV] = Morikawa.isValidLED();
            break;
            
        case VMFunc_isValidTone:
            state->reg[REG_RETV] = Morikawa.isValidTone();
            break;
            
        case VMFunc_isValidDigitalker:
            state->reg[REG_RETV] = Morikawa.isValidDigitalker();
            break;
            
        case VMFunc_isValidCamera:
            state->reg[REG_RETV] = Morikawa.isValidCamera();
            break;
            
        case VMFunc_isBusyDigitalker:
        {
            bool flag;
            state->reg[REG_ERRN] = Morikawa.isBusyDigitalker(&flag);
            state->reg[REG_RETV] = flag;
        }
            break;
            
        case VMFunc_hasTelemetryUpdate:
            state->reg[REG_RETV] = Morikawa.hasTelemetryUpdate();
            break;
            
        case VMFunc_hasAbnormalShutdown:
            state->reg[REG_RETV] = Morikawa.hasAbnormalShutdown();
            break;
            
        case VMFunc_shutdown:
            Morikawa.shutdown();
            break;
            
        case VMFunc_writeEEPROM:
        {
            unsigned long address = state->reg[REG_ARG0];
            unsigned int size = state->reg[REG_ARG1];
            unsigned int result = 0;
            state->reg[REG_ERRN] = Morikawa.writeEEPROM(address, heap, size, &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_readEEPROM:
        {
            unsigned long address = state->reg[REG_ARG0];
            unsigned int size = state->reg[REG_ARG1];
            unsigned int result = 0;
            state->reg[REG_ERRN] = Morikawa.readEEPROM(address, heap, size, &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_formatEEPROM:
            state->reg[REG_ERRN] = Morikawa.formatEEPROM();
            break;
            
        case VMFunc_writeSharedMemory:
        {
            unsigned long address = state->reg[REG_ARG0];
            unsigned int size = state->reg[REG_ARG1];
            unsigned int result = 0;
            state->reg[REG_ERRN] = Morikawa.writeSharedMemory(address, heap, size, &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_readSharedMemory:
        {
            unsigned long address = state->reg[REG_ARG0];
            unsigned int size = state->reg[REG_ARG1];
            unsigned int result = 0;
            state->reg[REG_ERRN] = Morikawa.readSharedMemory(address, heap, size, &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_formatSharedMemory:
            state->reg[REG_ERRN] = Morikawa.formatSharedMemory();
            break;
            
        case VMFunc_writeFRAM:
        {
            unsigned long address = state->reg[REG_ARG0];
            unsigned int size = state->reg[REG_ARG1];
            unsigned int result = 0;
            state->reg[REG_ERRN] = Morikawa.writeFRAM(address, heap, size, &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_readFRAM:
        {
            unsigned long address = state->reg[REG_ARG0];
            unsigned int size = state->reg[REG_ARG1];
            unsigned int result = 0;
            state->reg[REG_ERRN] = Morikawa.readFRAM(address, heap, size, &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_formatFRAM:
            state->reg[REG_ERRN] = Morikawa.formatFRAM();
            break;
            
        case VMFunc_writeFlashROM:
        {
            unsigned long address = state->reg[REG_ARG0];
            unsigned int size = state->reg[REG_ARG1];
            unsigned int result = 0;
            state->reg[REG_ERRN] = Morikawa.writeFlashROM(address, heap, size, &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_readFlashROM:
        {
            unsigned long address = state->reg[REG_ARG0];
            unsigned int size = state->reg[REG_ARG1];
            unsigned int result = 0;
            state->reg[REG_ERRN] = Morikawa.readFlashROM(address, heap, size, &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_formatFlashROM:
            state->reg[REG_ERRN] = Morikawa.formatFlashROM();
            break;
            
        case VMFunc_playFrequency:
            state->reg[REG_ERRN] = Morikawa.playFrequency(state->reg[REG_ARG0], state->reg[REG_ARG1]);
            break;
            
        case VMFunc_playFrequency2:
        {
            FrequencySequence const* seq = reinterpret_cast<FrequencySequence const*>(heap);
            state->reg[REG_ERRN] = Morikawa.playFrequency(seq, state->reg[REG_ARG0]);
        }
            break;
            
        case VMFunc_playNote:
            state->reg[REG_ERRN] = Morikawa.playNote(state->reg[REG_ARG0], state->reg[REG_ARG1], state->reg[REG_ARG2]);
            break;
            
        case VMFunc_playNote2:
        {
            NoteSequence const* seq = reinterpret_cast<NoteSequence const*>(heap);
            state->reg[REG_ERRN] = Morikawa.playNote(seq, state->reg[REG_ARG0]);
        }
            break;
            
        case VMFunc_playMorse:
            state->reg[REG_ERRN] = Morikawa.playMorse(state->reg[REG_ARG0], heap, state->reg[REG_ARG1]);
            break;
            
        case VMFunc_speakPhrase:
            state->reg[REG_ERRN] = Morikawa.speakPhrase(heap, state->reg[REG_ARG0]);
            break;
            
        case VMFunc_waitPhrase:
            state->reg[REG_ERRN] = Morikawa.waitPhrase();
            break;
            
        case VMFunc_stopPhrase:
            state->reg[REG_ERRN] = Morikawa.stopPhrase();
            break;
            
        case VMFunc_snapshotCamera:
        {
            unsigned long result = 0;
            state->reg[REG_ERRN] = Morikawa.snapshotCamera(state->reg[REG_ARG0], state->reg[REG_ARG1],
                                                           state->reg[REG_ARG2], state->reg[REG_ARG3], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_freezeFastLZ:
        {
            unsigned long result = 0;
            state->reg[REG_ERRN] = Morikawa.freezeFastLZ(state->reg[REG_ARG0], state->reg[REG_ARG1], state->reg[REG_ARG2],
                                                         state->reg[REG_ARG3], state->reg[REG_ARG4], state->reg[REG_ARG5],
                                                         state->reg[REG_ARG6], state->reg[REG_ARG7], state->reg[REG_ARG8], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_meltFastLZ:
        {
            unsigned long result = 0;
            state->reg[REG_ERRN] = Morikawa.meltFastLZ(state->reg[REG_ARG0], state->reg[REG_ARG1], state->reg[REG_ARG2],
                                                       state->reg[REG_ARG3], state->reg[REG_ARG4], state->reg[REG_ARG5], &result);
            state->reg[REG_RETV] = result;
        }
            break;
            
        case VMFunc_enableAudioBus:
            state->reg[REG_ERRN] = Morikawa.enableAudioBus();
            break;
            
        case VMFunc_disableAudioBus:
            Morikawa.disableAudioBus();
            break;
            
        default:
            VM_ERROR(ERR_UNKNOWN_FUNCTION, state);
            ;
    }
}
