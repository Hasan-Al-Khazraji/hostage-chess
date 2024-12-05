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
                    checkKingBeforeRedirect();
                }
            },
            error: function () {
                console.log("I AM AN ERROR");
            }
        });
    }

    function checkWinner() {
        $.ajax({
            url: '/check_winner',
            method: 'GET',
            data: { game_no: game_no },
            success: function (response) {
                if (response.winner && response.winner !== "null") {
                    window.location.href = `/winner.html?game_no=${game_no}&winner=${response.winner}`;
                } else {
                    checkNewMove();
                }
            },
            error: function () {
                console.log("I AM AN ERROR");
            }
        });
    }

    function checkKing() {
        $.ajax({
            url: '/check_king',
            method: 'GET',
            data: { game_no: game_no, turn_no: turn_no },
            success: function (response) {
                if (response.kingTaken && response.kingTaken !== "null") {
                    window.location.href = `/winner.html?game_no=${game_no}&winner=${response.winner}`;
                }
                checkWinner();
            },
            error: function () {
                console.log("I AM AN ERROR");
            }
        });
    }

    function checkKingBeforeRedirect() {
        $.ajax({
            url: '/check_king',
            method: 'GET',
            data: { game_no: game_no, turn_no: turn_no + 1 },
            success: function (response) {
                if (response.kingTaken && response.kingTaken !== "null") {
                    window.location.href = `/winner.html?game_no=${game_no}&winner=${response.winner}`;
                } else {
                    checkWinnerBeforeRedirect()
                }
            },
            error: function () {
                console.log("I AM AN ERROR");
            }
        });
    }

    function checkWinnerBeforeRedirect() {
        $.ajax({
            url: '/check_winner',
            method: 'GET',
            data: { game_no: game_no, turn_no: turn_no + 1 },
            success: function (response) {
                if (response.winner && response.winner !== "null") {
                    window.location.href = `/winner.html?game_no=${game_no}&winner=${response.winner}`;
                } else {
                    window.location.href = `/player.html?game_no=${game_no}&turn_no=${turn_no + 1}`;
                }
            },
            error: function () {
                console.log("I AM AN ERROR");
            }
        });
    }

    setInterval(function () {
        checkKing();
    }, 1000);
});