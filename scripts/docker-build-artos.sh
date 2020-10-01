#!/usr/bin/env bash

set -e

cd /artos

rm -rf build
for board in `ls boards`
do
  if [ -f boards/$board/config.ini ]
  then
    # print header
    printf "\033[35;1m"
    echo ""
    echo "================================================================="
    echo "                  Building for board: $board"
    echo "                         ./build/$board"
    echo "================================================================"
    echo ""
    printf "\033[0m"

    # run meson
    printf "\033[36;1m"
    meson build/$board --cross-file boards/$board/config.ini
    echo ""
    printf "\033[0m"

    # run ninja
    printf "\033[33;1m"
    ninja -C build/$board
    printf "\033[0m"

    # run tests (TODO)

    # store in /usr/bin
    GDB="gdb-multiarch -q -x /artos/boards/$board/gdb.cmd"
    OUT="/usr/bin/$board"
    echo '#!/usr/bin/env bash'                                        > $OUT
    echo ''                                                          >> $OUT
    echo 'export WINEDEBUG=-all'                                     >> $OUT
    echo 'printf "\\033[32;1m"'                                      >> $OUT
    echo 'echo "Starting GDB webserver..."'                          >> $OUT
    echo 'gdbgui --host 0.0.0.0 -g "'$GDB'" &> /dev/null &'          >> $OUT
    echo 'PWD=$!'                                                    >> $OUT
    echo 'echo "GDB Webserver URL: http://127.0.0.1:5000/dashboard"' >> $OUT
    echo 'wine64 /artos/build/'$board'/*.exe $@'                     >> $OUT
    echo 'kill -9 $PWD'                                              >> $OUT
    echo 'printf "\\033[0m"'                                         >> $OUT
    echo ''                                                          >> $OUT
    chmod +x /usr/bin/$board
  fi
done

printf "\033[35;1m"
echo ""
echo "================================================================="
echo ""
printf "\033[0m"
