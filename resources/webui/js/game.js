function SwitchState(state) {
	Engine.Log(state);
	$('.all').hide();
	$(state).show();
}

jQuery(document).ready(function ($) {
    $('#settings').click(function () {
        $('.settings').toggle();
		$('#audio').hide();
		$('#controls').hide();
		Engine.Log("test");
    });
});