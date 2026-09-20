#!/usr/bin/env sh

mapfile -t LASTNAMES < ./assets/lastnames.txt
mapfile -t F_FIRSTNAMES < ./assets/f_firstnames.txt
mapfile -t F_MIDDLENAMES < ./assets/f_middlenames.txt
mapfile -t M_FIRSTNAMES < ./assets/m_firstnames.txt
mapfile -t M_MIDDLENAMES < ./assets/m_middlenames.txt
mapfile -t DOMAINS < ./assets/domains.txt
mapfile -t OPERATORS < ./assets/ops.txt

gender=$((RANDOM % 2)) # 0 - male, 1 - female

lastname="${LASTNAMES[$RANDOM % ${#LASTNAMES[@]}]}"
if [ $gender -eq 1 ]; then
    lastname="${lastname}a"
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
email="$email_base$RANDOM@$domain"

echo "$lastname $firstname $middlename $email"
