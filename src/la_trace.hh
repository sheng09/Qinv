
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

void bandpass_bu(float *trace, int len, float delta,
    float f1, float f2, int ord, int pass);
float ave(float *trace, long len);
int rmean(float *trace, long len);
int linReg(float *trace, long len, float *k, float *b);
int detrend(float *trace, long len);

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */
