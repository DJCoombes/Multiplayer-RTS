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

function ShowDebug() {
	$('.debug').show();
}

function HideDebug() {
	$('.debug').hide();
}

function UpdateData(received, sent) {
	$('#dataReceived').text("Data received: " + received + "KB");
	$('#dataSent').text("Data sent: " + sent + "KB");
}

jQuery(document).ready(function ($) {
    $('#settings').click(function () {
        $('.settings').toggle();
		$('#audio').hide();
		$('#controls').hide();
		Engine.Log("test");
    });

	HideDebug();
});