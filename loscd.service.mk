[Unit]
Descrition=Login with your smartcard

[Service]
ExecStart=$(DESTDIR)/loscd
Restart=always
User=$(USER)
Group=$(GROUP)

[Install]
WantedBy=multi-user.target
