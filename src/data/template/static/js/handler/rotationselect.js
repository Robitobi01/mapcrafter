RotationSelectHandler.prototype = new BaseHandler();

/**
 * Updates the control widget with the map rotation selection.
 */
function RotationSelectHandler(control) {
	this.control = control
}

RotationSelectHandler.prototype.create = function() {
};

RotationSelectHandler.prototype.onMapChange = function(name, rotation) {
	this.update();
};

RotationSelectHandler.prototype.update = function(text) {
	var config = this.ui.getCurrentMapConfig();
	var currentRotation = this.ui.getCurrentRotation();
	if(config.renderView == "isometric") {
        var j = 4;
        for(var i = 0; i < 4; i++) {
            this.control.buttons[i].style.display = "none";
        }
    } else {
        var j = 0;
        for(var i = 4; i < 8; i++) {
            this.control.buttons[i].style.display = "none";
        }
    }
    for(var i = 0 + j; i < 4 + j; i++) {
        // hide buttons where rotations don't exist, set active/not active for others
        var button = this.control.buttons[i];
        if(config.rotations.indexOf(i - j) == -1) {
            button.style.display = "none";
        } else {
            button.style.display = "inline";
            if(i == currentRotation) {
                button.setAttribute("class", "btn btn-default active");
            } else {
                button.setAttribute("class", "btn btn-default");
            }
        }
    }
};

