(function (window) {
    'use strict';

    function Controller(model, view) {
        this.model = model;
        this.view = view;

        this.$main = $$('main');
        this.$sequences = $$('sequences');

        window.addEventListener('load', function () {
            this.showAllSequences();
            console.log('LOADED');
        }.bind(this));
    }

    Controller.prototype.showAllSequences = function () {
        this.model.loadSequences(function (sequences) {
            console.log('LOADED SEQUENCES: ' + sequences);
            this.$sequences.innerHTML = this.view.show(sequences);
        }.bind(this));
    };

    Controller.prototype.addSequence = function () {
        this.model.createSequence(undefined, function (sequences) {
            this.$sequences.innerHTML += this.view.show(sequences);
        }.bind(this));
    };

    window.app.Controller = Controller;
})(window);
