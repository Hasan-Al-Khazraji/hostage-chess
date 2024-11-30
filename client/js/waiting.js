$(document).ready(function () {
    var game_no = window.gameNumber; // set through da python
    var turn_no = 1;

    function checkOpponent() {
        $.ajax({
            url: '/check_opponent',
            method: 'GET',
            data: { game_no: game_no },
            success: function (response) {
                console.log(response);
                if (response.black_handle && response.black_handle !== "null") {
                    window.location.href = `/player.html?game_no=${game_no}&turn_no=${turn_no}`;
                } else {
                    setTimeout(checkOpponent, 1000);
                }
            },
            error: function () {
                console.log("I AM AN ERROR");
                setTimeout(checkOpponent, 1000);
            }
        });
    }

    checkOpponent();
});