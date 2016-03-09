(function () {
    'use strict';

    function Nep(name) {
        this.storage = new app.Store(name);
        this.model = new app.Model(this.storage);
        this.view = new app.View();
        this.controller = new app.Controller(this.model, this.view);
    }

    var instance = new Nep('default');

    $$('button').addEventListener('click', function (e) {
        instance.controller.addSequence();
    });
})();
