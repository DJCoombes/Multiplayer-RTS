function SwitchState(state) {
	Engine.Log(state);
	$('.all').hide();
	var type = ".";
	var stateName = type.concat(state);

	$(stateName).show();
	Engine.Log(stateName);
}

function settings(button) {
	$('#graphics').hide();
	$('#audio').hide();
	$('#controls').hide();
	$(button).show();
}

jQuery(document).ready(function ($) {
    $('#settings').click(function () {
        $('.settings').toggle();
		$('#audio').hide();
		$('#controls').hide();
		Engine.Log("test");
    });
});