MousePosControl.prototype = new BaseControl("MousePosControl");

/**
 * This control widget shows the current position of the mouse in Minecraft.
 */
function MousePosControl() {
}

MousePosControl.prototype.create = function(wrapper) {
    var message = document.createElement("p");
    message.setAttribute("id", "message-box");
    message.setAttribute("class", "btn");
    message.style.display = "none";
    message.innerHTML ="Text copied!"
	wrapper.appendChild(message);

	var text = document.createElement("span");
	text.setAttribute("id", "mouse-move-div");
    text.innerHTML = '<div class="btn-group" role="group">'
			+ '<button type="button" class="btn btn-default" id="btn-x" onclick="MousePosControl.prototype.copyId(\'btn-x\')"></button>'
			+ '<button type="button" class="btn btn-default" id="btn-z" onclick="MousePosControl.prototype.copyId(\'btn-z\')"></button>'
            + '<button type="button" class="btn btn-default" id="btn-mca" onclick="MousePosControl.prototype.copyId(\'btn-mca\')"></button>'
			+ "</div>";

	var updatePos = function(ui) {
		return function(event) {
			var xzy = ui.latLngToMC(event.latlng, 64);
            document.getElementById("btn-x").innerHTML = 'X: ' + Math.round(xzy[0])
            document.getElementById("btn-z").innerHTML = 'Z: ' + Math.round(xzy[1])
            document.getElementById("btn-mca").innerHTML = 'r.' + (xzy[0] >> 9) + '.' + (xzy[1] >> 9) + '.mca'
        };
	}(this.ui);
	this.ui.lmap.on("mousemove", updatePos);
	this.ui.lmap.on("mousedown", updatePos);
	wrapper.appendChild(text);
};

MousePosControl.prototype.getName = function() {
	return 'mouse-pos';
};

MousePosControl.prototype.copyId = function(id) {
	toClipboard(getValue(id));
};

MousePosControl.prototype.copyPos = function() {
    var value = getValue('btn-x');
    value += ' 100 ' + getValue('btn-z');
    toClipboard(value);
};

function getValue(id) {
    var value = document.getElementById(id).innerHTML;
    if (value.includes(' ')) {
        value = value.split(' ', 2)[1];
    }
    return value;
}

function toClipboard(value) {
    var text = document.createElement('textarea');
    text.value = value;
    document.body.appendChild(text);
    text.select();
    document.execCommand('copy');
    document.body.removeChild(text);
    var box = document.getElementById('message-box')
    box.innerHTML = "Copied: " + value;
    box.style.display = "block";
    setTimeout(function(){box.style.display = "none";}, 1000);
}
