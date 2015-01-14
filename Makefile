#
# Copyright 2015 readaout
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# readaout Makefile
#

BIN		:= readaout
AOUT		:= handcrafted_aout
CC		:= gcc
AS		:= nasm
CFLAGS		:= -Wall -Wextra -O2 --std=c11

PHONY: all

all:
	@echo "* Compiling"
	@$(CC) $(CFLAGS) $(BIN).c -o $(BIN)
	@$(AS) -f bin $(AOUT).asm -o $(AOUT)
	@chmod +x $(AOUT)
clean:
	@echo "* Cleaning"
	@rm $(BIN)
	@rm $(AOUT)

test: all
	@echo "* Running"
	@echo
	./$(BIN) $(AOUT)
