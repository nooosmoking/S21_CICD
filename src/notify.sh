if [[ $CI_JOB_STATUS == 'success' &&  $CI_JOB_NAME == 'deploy'  ]] ; then
    CI_JOB_STATUS="SUCCESS✅"
else
    CI_JOB_STATUS="FAILED⛔️"
fi

if [[ $CI_JOB_NAME != 'deploy' ]]; then
    CI_JOB_NAME='CI'
fi

URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
TEXT="Commit: $CI_COMMIT_MESSAGE%0A%0AJob name: $CI_JOB_NAME%0A%0AStatus: $CI_JOB_STATUS%0A%0AProject: $CI_PROJECT_NAME"

curl -s -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null