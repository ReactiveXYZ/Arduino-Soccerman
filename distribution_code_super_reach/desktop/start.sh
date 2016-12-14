echo "Open the score counter page as well?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) open "score.html"; break;;
        No ) break;;
    esac
done
gunicorn -k eventlet -w 1 server:app &
python server.py