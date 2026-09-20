#!/usr/bin/env bash

cd "$(dirname "$0")"

uppercase_rand() {
    local arg="$1"
    local len=${#arg}
    local result=""
    local chance="${2:-50}"
    local char
    for ((i = 0; i < len; i++)); do
        char="${arg:i:1}"
        if (( RANDOM % 100 < chance )); then
            result+="${char^^}"
        else
            result+="${char}"
        fi
    done

    echo "$result"
}

function dirt {
    local arg="$1"
    local len=${#arg}
    local chance="${2:-20}"
    local res=""
    local char
    for ((i=0; i<len; i++)); do
        char="${arg:i:1}"
        res+="$char"
    if (( RANDOM % 100 < chance )); then
            res+="$((RANDOM % 10))"
        fi
    done

    echo "$res"
}

mapfile -t LASTNAMES < ../assets/lastnames.txt
mapfile -t F_FIRSTNAMES < ../assets/f_firstnames.txt
mapfile -t F_MIDDLENAMES < ../assets/f_middlenames.txt
mapfile -t M_FIRSTNAMES < ../assets/m_firstnames.txt
mapfile -t M_MIDDLENAMES < ../assets/m_middlenames.txt
mapfile -t DOMAINS < ../assets/domains.txt
mapfile -t OPERATORS < ../assets/ops.txt

gender=$((RANDOM % 2)) # 0 - male, 1 - female

lastname="${LASTNAMES[$RANDOM % ${#LASTNAMES[@]}]}"
if [ $gender -eq 1 ]; then
    lastname="${lastname}а"
fi

if [ $gender -eq 0 ]; then
    firstname="${M_FIRSTNAMES[$RANDOM % ${#M_FIRSTNAMES[@]}]}"
    middlename="${M_MIDDLENAMES[$RANDOM % ${#M_MIDDLENAMES[@]}]}"
else
    firstname="${F_FIRSTNAMES[$RANDOM % ${#F_FIRSTNAMES[@]}]}"
    middlename="${F_MIDDLENAMES[$RANDOM % ${#F_MIDDLENAMES[@]}]}"
fi

email_base=$(sed 'y/абвгдеёжзийклмнопрстуфхцчшщъыьэюя/abvgdeejziyklmnoprstufhccss_y_eua/' <<< "${lastname,,}")
domain="${DOMAINS[$RANDOM % ${#DOMAINS[@]}]}"
email_base=$(uppercase_rand "$email_base" 30)
email="$email_base$RANDOM@$domain"

op="${OPERATORS[$RANDOM % ${#OPERATORS[@]}]}"
phone="+7${op}-$((RANDOM % 900 + 100))-$((RANDOM % 90 + 10))-$((RANDOM % 90 + 10))"

year=$((RANDOM % 55 + 1950))
month=$((RANDOM % 12 + 1))
day=$((RANDOM % 28 + 1))
date=$(printf "%d-%02d-%02d" "$year" "$month"  "$day")

lastname=$(uppercase_rand "$lastname" 30)
lastname=$(dirt "$lastname" 20)
middlename=$(uppercase_rand "$middlename" 30)
middlename=$(dirt "$middlename" 10)
firstname=$(uppercase_rand "$firstname" 20)
firstnamename=$(dirt "$firstname" 5)

echo "$lastname $firstname $middlename $email $phone $date"
