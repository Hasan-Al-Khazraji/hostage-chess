$(document).ready(function () {
    var game_no = window.gameNumber;
    var turn_no = window.turnNumber;

    function checkNewMove() {
        $.ajax({
            url: '/check_new_move',
            method: 'GET',
            data: { game_no: game_no, turn_no: turn_no },
            success: function (response) {
                if (response.new_move && response.new_move !== "null") {
                    window.location.href = `/player.html?game_no=${game_no}&turn_no=${turn_no + 1}`;
                } else {
                    setTimeout(checkNewMove, 1000);
                }
            },
            error: function () {
                console.log("I AM AN ERROR");
                setTimeout(checkNewMove, 1000);
            }
        });
    }

    checkNewMove();
});