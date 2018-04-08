#
# Makefile for libmysock.a
#

CC = gcc
AR = ar

# SRCSはソース・ファイル名のリスト
SRCS = setup_client.c setup_server.c recv_line.c send_n.c recv_n.c

# OBJSには、SRCSリストの拡張子(.c)を .o に置き換えたものを代入する
# 註: この記法(置換)はGNU make個有
OBJS = $(SRCS:.c=.o)

HEADER = mysock.h

#
# Makefileの書き方
# ターゲット : ターゲットが依存するファイルやターゲットのリスト
#	ターゲットを作成するのに必要となるコマンドのリスト(行頭は必ずTAB)
#

# makeに引数を与えずに実行すると、Makefileで最初に現われたターゲット(この場合
# all)がターゲットとなる
all : libmysock.a

libmysock.a : $(SRCS) $(OBJS) $(HEADER)
	$(AR) crv $@ $(OBJS)

clean :
	-$(RM) $(OBJS)
