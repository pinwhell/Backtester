
#define _CRT_SECURE_NO_WARNINGS

#include "PairProvider.h"
#include "csv.h"

PairProvider gPairsProvider;

bool fileExist(const std::string& name) {
    if (FILE* file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}

bool PairProvider::LoadPairFromFile(const std::string& pairName, const std::string& csvPath)
{
    if (mPairs.find(pairName) != mPairs.end())
        return true;

    if (fileExist(csvPath) == false)
    {
        printf("File \"%s\" Not Exist", csvPath.c_str());
        return false;
    }

    mPairs[pairName] = std::make_shared<CandleGroup>();

    LoadTOHLCVFromCSV(csvPath, mPairs[pairName]);

    if (mPairs[pairName]->mCandlesDeque.empty())
        printf("Warning: Pair %s with no candles\n", pairName.c_str());

    return true;
}

bool PairProvider::LoadTOHLCVFromCSV(const std::string& csvPath, std::shared_ptr<CandleGroup> outCandles)
{
    io::CSVReader<6> in(csvPath);

    in.read_header(io::ignore_extra_column, "time", "open","high", "low", "close", "volume");

    long long time;
    double open, close, high, low, volume;

    for (int i = 0; ; i++)
    {
        if (!in.read_row(time, open, high, low, close, volume))
            break;

        outCandles->AddCandle(std::move(std::make_unique<Candle>(0, time, open, high, low, close, volume)));
    }

    size_t i = 0;

    for (auto& cand : outCandles->mCandlesDeque)
        cand->mId = i++;

    return true;
}

std::shared_ptr<CandleGroup> PairProvider::getCandleGroup(const std::string& pair)
{
    return mPairs[pair];
}

bool PairProvider::IsPairLoaded(const std::string& pairName)
{
    return mPairs.find(pairName) != mPairs.end();
}
