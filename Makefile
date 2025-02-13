.PHONY: counter alert
counter:
	make counter -C counter

alert:
	make alert -C alert

runcounter:
	make run -C counter
runalert:
	make run -C alert
