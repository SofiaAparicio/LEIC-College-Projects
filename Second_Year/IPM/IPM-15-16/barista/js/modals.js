
// When the user clicks on the button, open the modal
function not_implemented_fn() {
    $("#not_implemented_box").css("display", "block");
}

function help_fn(id) {
    $(id).css("display", "block");
}

$(document).ready(function() {

// When the user clicks on <span> (x), close the modal
$('.close').click(function() {
    $(".modal").css("display", "none");
});

$('.closecart').click(function() {
    $('#cart').css('display', 'none');
});

// When the user clicks anywhere outside of the modal, close it
$("body").click(function( event ){
    if( $(event.target ).is('.modal') ){
        $(".modal").css("display", "none");
    }
});

});