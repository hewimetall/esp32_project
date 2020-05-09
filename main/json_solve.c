#include "head.h"
static const char *TAG = "Solve json";

static void cp_str_json(char *source, char *dst, size_t len) {
	source += 2;
	int i = 0;
	for (i = 0; (*source == ',' || *source == '}') == 0; source++, i++) {
		if (i > (len - 1))
			break;
		dst[i] = *source;
	}
	dst[i] = '\0';
}

esp_err_t solve_json(char *istr) {
	struct {
		char x[10];
		char y[10];
		char z[10];
	} adm;
	uint8_t test_date=0;

	for (; *istr != '\0'; istr++) {
        if(*istr == '}') break;

		if (*istr == '{' || *istr == ',') {
			istr++;
			switch (*istr) {
			case 'x':
				test_date=1;
				cp_str_json(istr, &adm.x, 6);
				break;
			case 'y':
				test_date++;
				cp_str_json(istr, &adm.y, 6);

				break;
			case 'z':
				test_date++;
				cp_str_json(istr, &adm.z, 6);
				break;
			default:
				ESP_LOGW(TAG,"WARM UNCHARM SUM %c ",*istr);
				break;
			}
		}
	}
	if(test_date != 3){
		ESP_LOGE(TAG,"WARM DATE BAGS TEST DATE == %i",test_date);
		return ESP_FAIL;
	}
	ESP_LOGI(TAG,"DATE: X:%s : Y:%s : Z:%s", adm.x, adm.y, adm.z);
	return ESP_OK;
}
