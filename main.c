#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t berber;//Berber semaforumuzu tanýmladýk.

void *Berber();//Fonksiyon prototiplerimizi tanýmladýk
void *Musteri();

//Kullandýðýmýz deðiþkenleri tanýmladýk
int gelenMusteri = 0;//O an gelen müþterinin günlük sýrasý
int bosKoltuk =5;//Dükkanda boþ bulunan koltuk sayýsý.5 i geçemez.
int bekleyenMusteriler[5];//Bekleme salonunda bulunan müþterilerin sýrayla tutulduðu dizi(kuyruk þeklinde)
int siradakiMusteri = -1;//Berberin salonda sýradaki müþteriyi bulmasý için kullandýk
int sonGelenMusteri = -1;//Yeni gelen müþterinin dizide sýradaki indise yerleþmesi için kullandýk

int main()
{
    srand(time(NULL));//Her defasýnda farklý deðer üretmek için
    pthread_t berberThread,musteriThread[20];//Thread tanýmlamalarýmýzý yaptýk, 1 Berber 20 Müþteri Thread'i

    sem_init(&berber,1,0);//Berber semaforumuzu burda oluþturduk.

    pthread_create(&berberThread,NULL,Berber,NULL);//Berber thread imizi oluþturduk.
    int j=0;
    for(j=0;j<20;j++)
    {
        pthread_create(&musteriThread[j],NULL,Musteri,NULL);//Müþteri threadlerimizi oluþturduk
    }

    pthread_join(berberThread,NULL);//Join iþlemini ayrýca yaptýk.
    for(j=0;j<20;j++)
    {
        pthread_join(musteriThread[j],NULL);
    }


    return 0;
}

void *Musteri()
{


        int sure = 1 + rand()%20;//Müþterilerin rastgele sürelerde dükkana gelmesini saðladýk.
        sleep(sure);

        gelenMusteri++;//O sýrada dükkana yeni gelen müþterinin numarasýný ayarladýk.
        printf("%d. musteri dukkana geldi.\n",gelenMusteri);
        if(bosKoltuk==5)//Eðer o sýrada bütün koltuklar boþsa berber uyandýrýlýyor.
        {
            sem_post(&berber);
            printf("Berber uyandi\n");
        }

        if(bosKoltuk>0)//Müþteri dükkandaki boþ koltuða oturuyor ve bekleyenMusteriler dizisine yerleþtiriliyor.
        {
            sonGelenMusteri = (sonGelenMusteri+1)%5;//Yeni gelen müþterinin dizinin bir sonraki indisine yerleþmesini saðladýk.
            bosKoltuk--;
            bekleyenMusteriler[sonGelenMusteri] = gelenMusteri;

            printf("%d. musteri bekleme salonuna oturdu.\n",gelenMusteri);

        }
        else//Bütün koltuklar doluysa müþteri dükkandan ayrýlýyor.
        {
            printf("%d. musteri yer bulamadi.Dukkandan ayriliyor.\n",gelenMusteri);
        }


}

void *Berber()
{
    while(1)
    {
        //printf("Bos koltuk sayisi : %d \n",bosKoltuk);

        if(bosKoltuk==5)//Salonun dolu olup olmadýðýný kontrol ettik. Boþ ise sem_wait ile berberin uyumasýný saðladýk.
        {
            printf("Berber uyumaya gitti\n");
            sem_wait(&berber);
        }

        siradakiMusteri = (siradakiMusteri + 1) % 5;//Berberin saçýný keseceði sýradaki müþteriyi dizinin bir sonraki indisinden bulduk. Kuyruk yapýsý olduðu için mod aldýk.


        printf("berber %d. musterinin sacini kesmeye basladi.\n",bekleyenMusteriler[siradakiMusteri]);

        sleep(1);//Berberin müþteriyi 1 sn de traþ etmesini saðladýk
        printf("berber %d. musterinin sacini kesmeyi bitirdi\n",bekleyenMusteriler[siradakiMusteri]);
        bosKoltuk++;//Koltuðu boþaltýyoruz.

    }
}


